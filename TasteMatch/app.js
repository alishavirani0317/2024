//express backend
const express = require("express");
const cors = require("cors");
const dotenv = require("dotenv");
const connectDB = require("./backend/config/db");

//routes
const userRoutes = require("./backend/routes/userRoutes");
const restaurantRoutes = require("./backend/routes/restaurantRoutes");
const reviewRoutes = require("./backend/routes/reviewRoutes");
const friendRoutes = require("./backend/routes/friendRoutes");


//Load environment variables
dotenv.config()

//initialize express app
const app = express();

// Body parser for JSON
app.use(express.json())

// Enable CORS
app.use(cors({
    origin: 'http://localhost:3000',
    credentials: true
  }));
  
// Connect to MongoDB
connectDB();

// API Routes
app.use("/api", userRoutes);
app.use("/api", restaurantRoutes);
app.use("/api", reviewRoutes);
app.use("/api/friends", friendRoutes);

app.get("/", (req, res) => {
    res.send("Working");
});

// Server start
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});

