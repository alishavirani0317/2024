const express = require("express");
const router = express.Router();
const restaurantController = require("../controllers/restaurantController");

router.post("/restaurants", restaurantController.createRestaurant);
router.get("/restaurants", restaurantController.getAllRestaurants);

module.exports = router;
