package edu.illinois.cs.cs124.ay2023.mp.models;

import androidx.annotation.NonNull;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import org.jetbrains.annotations.NotNull;

/**
 * Model holding the course summary information shown in the summary list.
 *
 * @noinspection unused
 */
public class Summary implements Comparable<Summary> {
  private String subject;

  /**
   * Get the subject for this Summary.
   *
   * @return the subject for this Summary
   */
  @NotNull
  public final String getSubject() {
    return subject;
  }

  private String number;

  /**
   * Get the number for this Summary.
   *
   * @return the number for this Summary
   */
  @NotNull
  public final String getNumber() {
    return number;
  }

  private String label;

  /**
   * Get the label for this Summary.
   *
   * @return the label for this Summary
   */
  @NotNull
  public final String getLabel() {
    return label;
  }

  /** Create an empty Summary. */
  public Summary() {}

  /**
   * Create a Summary with the provided fields.
   *
   * @param setSubject the department for this Summary
   * @param setNumber the number for this Summary
   * @param setLabel the label for this Summary
   */
  public Summary(@NonNull String setSubject, @NonNull String setNumber, @NotNull String setLabel) {
    subject = setSubject;
    number = setNumber;
    label = setLabel;
  }

  /** {@inheritDoc} */
  @NonNull
  @Override
  public String toString() {
    return subject + " " + number + ": " + label;
  }

  @Override
  public int compareTo(Summary o) {
    // sort first by number, then by subject
    // how do we do this in the same method
    //
    if (this.number.equals(o.number)) {
      return this.subject.compareTo(o.subject);
    } else {
      return this.number.compareTo(o.number);
    }
  }

  public static List<Summary> filter(List<Summary> list, String filter) {
    // trim filter
    String newFilter = filter.trim().toLowerCase();
    List<Summary> newList = new ArrayList<>();
    for (int i = 0; i < list.size(); i++) {
      Summary use = list.get(i);
      String words = use.toString().toLowerCase();
      if (words.contains(newFilter)) {
        newList.add(use);
      }
    }
    Collections.sort(newList);
    // filter through newList a second time, sorting by number then subject
    newList.sort(
        (first, second) -> {
          // get position of filter string within subject string and then subtract them
          return first.toString().toLowerCase().indexOf(newFilter)
              - second.toString().toLowerCase().indexOf(newFilter);
        });
    return newList;
  }
}
