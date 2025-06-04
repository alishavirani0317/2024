const Review = require("../models/reviewModel");

exports.createReview = async (req, res) => {
    try {
        const { userId, restaurantId, rating, comment } = req.body;
        const review = new Review({
            user: userId,
            restaurant: restaurantId,
            rating,
            comment
        });
        await review.save();
        res.status(201).json({ message: "Review created", review });
    } catch (error) {
        res.status(500).json({ message: "Error creating review", error: error.message });
    }
};

exports.getReviewsForRestaurant = async (req, res) => {
    try {
        const { restaurantId } = req.params;
        const reviews = await Review.find({ restaurant: restaurantId }).populate("user", "name dietaryPreferences tier");
        res.json({ reviews });
    } catch (error) {
        res.status(500).json({ message: "Error fetching reviews", error: error.message });
    }
};
