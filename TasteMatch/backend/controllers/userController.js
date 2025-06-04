const userController = require('../controllers/userController');

exports.createUser = async (req, res) => {
    try {
        const{name, email, password} = req.body;

        //check is user exists
        const userExists = await User.findOne({email});
        if(userExists) return res.status(400).json({message: "User already exists"});

        //create user
        const newUser = new User({name, email, password});
        await newUser.save();

        res.status(201).json({message: "User created successfully", user: newUser});

    } catch(error) {
        res.status(500).json({message: "Error creating user", error});
    }
};

exports.getAllUsers = async (req, res) => {
    try{
        const users = await User.find();
        res.status(200).json({users});
    } catch(error) {
        res.status(500).json({message: "Error fetching users", error});
    }
};

exports.getUserById = async (req, res) => {
    try{
        const user = await User.findById(req.params.id);
        if (!user) return res.status(404).json({message: "User not found"});

        res.status(200).json({user});
    } catch(error) {
        res.status(500).json({message: "Error fetching user", error});
    }
};

exports.searchUsers = async (req, res) => {
    try{
        const query = req.query.query?.trim() ||"";
        if (!query) {
            return res.status(400).json({message: 'Search query is required' });
        }
        //limit query length to prevent abuse
        if (query.length > 50) {
            return res.status(400).json({message: 'Search query is too long' });
        }
        // case insensitive, anchored at start(better perfomance)
        const searchRegex = new RegExp(`^${query}`, "i");

        const users = await User.find({
            $or: [
                {name: {$regex: searchRegex} },
                {email: {$regex: searchRegex} },
                {password: {$regex: searchRegex} },
            ]
        })
        .select('-__v') // exclude sensitive fields
        .limit(20); // prevent too many results

        res.status(200).json(users);

    } catch(error) {
        console.error('Search error:', error);
        res.status(500).json({
            message: 'Error searching users',
            error: error.message
        });
    }
};

exports.getPaginatedUsers = async(req, res) => {
    try {
        const page = parseInt(req.query.page) || 1;
        const limit = parseInt(req.query.limit) || 10;
        const skip = (page - 1) * limit;

        const sortBy = req.query.sortBy || 'createdAt';
        const sortOrder = req.query.sortOrder == 'desc' ? -1 : 1;

        const users = await User.find()
            .sort({[sortBy]: sortOrder })
            .skip(skip)
            .limit(limit);

        const totalUsers = await User.countDocuments();
        const totalPages = Math.ceil(totalUsers / limit);
        
        res.json({
            data: users, // better response structure
            meta: {
                page,
                limit,
                totalItems: totalUsers,
                totalPages,
                hasNextPage: page < totalPages,
                hasPreviousPage: page > 1
            }
        });
    } catch (error) {
        console.error('List users error:', error);
        res.status(500).json({
            message: 'Error fetching users list',
            error: error.message
        });
    }
};

exports.searchAndPaginateUsers = async(req, res) => {
    try {
        const {
            query = '',
            page = 1,
            limit = 10,
            sortBy = 'createdAt',
            sortOrder = 'desc'
        } = req.query;
        // convert to nums
        const pageNum = Math.max(1, parseInt(page));
        const limitNum = Math.min(100, Math.max(1, parseInt(limit)));

        // build search conditions if query exists
        const searchConditions = query ? {
            $or: [
                { name: { $regex: query, $options: 'i' } },
                { email: { $regex: query, $options: 'i' } },
                { password: { $regex: query, $options: 'i' } }
            ]
        } : {};
        // execute query
        const [users, total] = await Promise.all([
            User.find(searchConditions)
            .sort({ [sortBy]: sortOrder == 'desc' ? -1 : 1})
            .skip((pageNum - 1) * limitNum)
            .limit(limitNum)
            .lean(),
        User.countDocuments(searchConditions)
        ]);
        const totalPages = Math.ceil(total / limitNum);
        return res.json({
            success: true, 
            data: users,
            pagination: {
                currentPage: pageNum,
                totalPages,
                totalItems: total,
                itemsPerPage: limitNum,
                hasNextPage: pageNum < totalPages,
                hasPreviousPage: pageNum > 1
            }
        });
    } catch(error) {
        console.error('User search error:', error);
        res.status(500).json({
            success: false,
            message: 'Failed to retrieve users',
            error: process.env.NODE_ENV === 'development' ? error.message : undefined
        });
    }
};


exports.updateUser = async(req, res) => {
    const {name, email, password} = req.body;
    try{
        const user = await User.findById(req.params.id);
        if(!user)
            return res.status(404).json({ message: 'User not found' });   
        
        if (name !== undefined) user.name = name;
        if (email !== undefined) user.email = email;
        if (password !== undefined) user.password = password;
        
        const updatedUser = await user.save();
        const userResponse = updatedUser.toObject();

        res.json({
            message: 'User updated succesfully',
            user: userResponse,
        });

    } catch (error) {
        console.error('User search error:', error);
        res.status(500).json({ message: error.message });
    }
};


exports.deleteUser = async(req, res) => {
    try{
        const user = await User.findById(req.params.id);
        if(!user) return res.status(404).json({ message: 'User not found' });   
        
        await user.deleteOne();
        res.json({message: 'User deleted successfully'});

    } catch (error) {
        res.status(500).json({ message: error.message });
    }
};

exports.deleteMultipleUsers = async(req, res) => {
    try{
        const { ids } = req.body;
        if(!Array.isArray(ids) || ids.length == 0) {
            return res.status(404).json({ success: false, message: 'No IDs provided' });   
        }
       const result = await User.deleteMany({ _id: {$in: ids } });
       res.status(200).json({
        success: true,
        message: `${result.deletedCount} users deleted succesfully`
       });
    
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
};