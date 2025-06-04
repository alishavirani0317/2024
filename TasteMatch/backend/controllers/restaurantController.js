const Restaurant = require("../models/restaurantModel");

exports.createRestaurant = async (req, res) => {
    try {
        const { name, address, cuisine } = req.body;
        const newRestaurant = new Restaurant({
            name,
            address,
            cuisine,
            createdBy: req.body.userId // if using auth
        });
        await newRestaurant.save();
        res.status(201).json({ message: "Restaurant added", restaurant: newRestaurant });
    } catch (error) {
        res.status(500).json({ message: "Error adding restaurant", error: error.message });
    }
};

exports.getAllRestaurants = async (req, res) => {
    try {
        const restaurants = await Restaurant.find();
        res.json({ restaurants });
    } catch (error) {
        res.status(500).json({ message: "Error fetching restaurants", error: error.message });
    }
};
