const express = require("express");
const router = express.Router();
const userController = require("../controllers/userController");

router.post("/users", userController.createUser); // Create user
router.get("/users", userController.getAllUsers); // View all users
router.get('/users/search', userController.searchUsers); // Search users
router.get('/users/paginated', userController.getPaginatedUsers); // Get users with pagination
router.get('/users/search-and-paginate', userController.searchAndPaginateUsers); // combine search & pagination
router.put("/users/:id", userController.updateUser); // update user by id
router.delete("/users/delete-multiple", userController.deleteMultipleUsers); // delete mult users by ids
router.delete("/users/:id", userController.deleteUser); // delete user by id
router.get("/users/:id", userController.getUserById); // View a specific user

module.exports = router;
