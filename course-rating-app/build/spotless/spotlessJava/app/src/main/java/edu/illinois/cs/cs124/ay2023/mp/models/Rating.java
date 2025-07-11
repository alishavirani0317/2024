package edu.illinois.cs.cs124.ay2023.mp.models;

public class Rating {
  public static final float NOT_RATED = -1.0f;
  private float rating = NOT_RATED;
  private Summary summary;

  public Rating() {}

  public Rating(Summary setSummary, float setRating) {
    rating = setRating;
    summary = setSummary;
  }

  public void setSummary(Summary setSummary) {
    summary = setSummary;
  }

  public float getRating() {
    return rating;
  }

  public Summary getSummary() {
    return summary;
  }
}
