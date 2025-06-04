const express = require("express");
const router = express.Router();
const friendController = require("../controllers/friendController");

// Add authentication middleware if needed
// const auth = require("../middleware/auth");

router.post("/request", /*auth,*/ friendController.sendFriendRequest);
router.post("/accept", /*auth,*/ friendController.acceptFriendRequest);
router.get("/mutual", /*auth,*/ friendController.getMutualFriends);

module.exports = router;
