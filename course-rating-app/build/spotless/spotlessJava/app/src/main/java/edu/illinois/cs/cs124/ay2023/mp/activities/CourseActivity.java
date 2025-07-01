package edu.illinois.cs.cs124.ay2023.mp.activities;

// import static com.android.volley.VolleyLog.TAG;
import static edu.illinois.cs.cs124.ay2023.mp.helpers.Helpers.OBJECT_MAPPER;
// import android.content.Intent;

import android.os.Bundle;
// import android.util.Log;
// import android.widget.SearchView;
import android.widget.RatingBar;
import android.widget.TextView;
import androidx.annotation.Nullable;
// import com.fasterxml.jackson.core.JsonProcessingException;
// import com.fasterxml.jackson.databind.ObjectMapper;
import androidx.appcompat.app.AppCompatActivity;
// import java.util.Collections;
// import java.util.List;
import edu.illinois.cs.cs124.ay2023.mp.R;
import edu.illinois.cs.cs124.ay2023.mp.application.CourseableApplication;
import edu.illinois.cs.cs124.ay2023.mp.helpers.ResultMightThrow;
import edu.illinois.cs.cs124.ay2023.mp.models.Course;
import edu.illinois.cs.cs124.ay2023.mp.models.Rating;
import java.util.function.Consumer;

// import javax.security.auth.callback.Callback;
// import javax.xml.transform.Result;

// import edu.illinois.cs.cs124.ay2023.mp.models.Summary;

public class CourseActivity extends AppCompatActivity {
  private Course course;
  private Rating rating;

  @Override
  protected void onCreate(@Nullable Bundle unused) {
    super.onCreate(unused); // gets called when method starts

    // Load this activity's layout and set the title
    setContentView(R.layout.activity_course);

    // Retrieve the Intent and Retrieve the Summary & Set Up UI
    String summaryString = getIntent().getStringExtra("summary");
    Course summaryCourse;
    try {
      // Convert String to Summary Object Using Deserialization
      summaryCourse = OBJECT_MAPPER.readValue(summaryString, Course.class);
    } catch (Exception e) {
      throw new IllegalArgumentException();
    }

    // Creating the RatingBar
    RatingBar ratingBar = findViewById(R.id.rating);
    CourseableApplication applicationR = (CourseableApplication) getApplication();
    applicationR.getClient().getRating(summaryCourse, ratingCallback);

    ratingBar.setOnRatingBarChangeListener(
        ((ratingBar1, rating1, fromUser) -> {
          if (rating1 < 1.0f) {
            Rating rate2 = new Rating(summaryCourse, -1.0f);
            applicationR.getClient().postRating(rate2, ratingCallback);
          } else {
            Rating rate3 = new Rating(summaryCourse, rating1);
            applicationR.getClient().postRating(rate3, ratingCallback);
          }
        }));

    // Make Request Using Client for Course Details
    CourseableApplication application = (CourseableApplication) getApplication();
    application.getClient().getRating(summaryCourse, ratingCallback);
    application.getClient().getCourse(summaryCourse, courseCallback);
  }

  // Update the UI with Details About the Course
  private final Consumer<ResultMightThrow<Course>> courseCallback =
      (result) -> {
        course = result.getValue();
        TextView descriptionTextView = findViewById(R.id.description);
        TextView titleTextView = findViewById(R.id.title);

        runOnUiThread(
            () -> {
              titleTextView.setText(course.toString());
              descriptionTextView.setText(course.getDescription());
            });
      };

  private final Consumer<ResultMightThrow<Rating>> ratingCallback =
      (result) -> {
        RatingBar ratings = findViewById(R.id.rating);
        this.rating = result.getValue();

        runOnUiThread(
            () -> {
              ratings.setRating(rating.getRating());
            });
      };
}
