const express = require("express");
const router = express.Router();
const reviewController = require("../controllers/reviewController");

router.post("/reviews", reviewController.createReview);
router.get("/reviews/:restaurantId", reviewController.getReviewsForRestaurant);

module.exports = router;
