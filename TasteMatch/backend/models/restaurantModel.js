const mongoose = require("mongoose");

const restaurantSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true,
    trim: true
  },
  address: {
    type: String,
    required: true,
    unique: true,
    lowercase: true,
    trim: true
  },
  cuisine: {
    type: String,
    required: true,
    trim: true
  },

  // 📍 Geolocation for map and nearby queries
  location: {
    type: {
      type: String,
      enum: ["Point"],
      default: "Point",
      required: true
    },
    coordinates: {
      type: [Number], // Format: [longitude, latitude]
      required: true
    }
  },

  //  Creator (optional for moderation/admin)
  createdBy: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User"
  },

  // Image or logo
  imageUrl: {
    type: String,
    default: ""
  }

}, { timestamps: true });

// Geospatial index for querying by location
restaurantSchema.index({ location: "2dsphere" });

const Restaurant = mongoose.model("Restaurant", restaurantSchema);
module.exports = Restaurant;
