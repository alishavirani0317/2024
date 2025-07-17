from decimal import Decimal
from django.db import models

# import built in django user model
from django.contrib.auth.models import User

# imports Django signals which will be used to ensure automatic updates for current_grade calculation
from django.db.models.signals import post_save, post_delete
from django.dispatch import receiver
from django.db import IntegrityError
from django.forms import ValidationError

# from decimal import Decimal

# define class model


class Class(models.Model):
    # define class variables
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    class_name = models.CharField(max_length=50)
    class_grade = models.DecimalField(max_digits=5, decimal_places=2)
    percent_goal = models.DecimalField(max_digits=5, decimal_places=2)  # the percentage of progress towards goal_grade
    recommendation = models.CharField(max_length=500)  # string to hold the recommendation
    goal_grade = models.DecimalField(max_digits=5, decimal_places=2)  # user inputed info (grade they are trying to achieve)

    # ensure uniqueness within each user
    class Meta:
        unique_together = ("user", "class_name")

    # to_string function returns the name of the class
    def __str__(self):
        return self.class_name

    # function that calculates the current grade using categories and saves
    # also stores new recommendation and percentage to goal based on class grade
    def calculate_class_grade(self):
        # get all the categories
        categories = self.categories.all()

        # store category info for recommendations
        lowest_category_name = ""
        lowest_category_grade = 100
        biggest_category_weight = 0
        biggest_category_weight_name = ""

        # grade calculation variables
        weighted_grade = 0
        weights = 0

        # loop through categories
        for category in categories:
            # calculate total grade and weights
            weighted_grade += category.category_grade * category.category_weightage
            weights += category.category_weightage

            # update the lowest category info
            if category.category_grade < lowest_category_grade:
                lowest_category_grade = category.category_grade
                lowest_category_name = category.category_name

            # update the biggest category weight info
            if category.category_weightage > biggest_category_weight:
                biggest_category_weight = category.category_weightage
                biggest_category_weight_name = category.category_name

        # set class grade
        if weights > 0:
            self.class_grade = weighted_grade / weights

            # update the percent goal based on the grade (current grade/goal_grade)
            self.percent_goal = (self.class_grade / Decimal(self.goal_grade)) * 100

            # if passed the goal, set to 100
            if self.percent_goal > 100:
                self.percent_goal = 100

            # update the category recommendation
            if self.percent_goal == 100:
                self.recommendation = "Awesome job, you reached your goal!! You should be really proud :)"
            elif self.percent_goal > 85:
                self.recommendation = "Keep up the great work! You are almost at your goal grade :)"
            elif self.percent_goal > 60:
                self.recommendation = "Try to improve scores in the " + lowest_category_name + " category. This will help get you closer to your goal"
            else:
                # if two vars of interest are the same, display single category
                if lowest_category_name == biggest_category_weight_name:
                    self.recommendation = "Try focusing on " + lowest_category_name + " as it is impacting your grade the most."
                # if different, display both options
                else:
                    self.recommendation = (
                        "Try focusing on either  "
                        + lowest_category_name
                        + " or "
                        + biggest_category_weight_name
                        + " as these are impacting your grade the most."
                    )

        # no categories (i.e. no grades inputted so current class grade is zero)
        else:
            # set values to zero
            self.class_grade = 0
            self.percent_goal = 0

            # update the recommendation
            self.recommendation = "Make sure to add in some class categories and assignments to take advantage of GradeBuddy!"
        # save all updates
        self.save()


# define catgeory model


class Category(models.Model):
    associated_class = models.ForeignKey(Class, related_name="categories", on_delete=models.CASCADE)  # Link to the Class model
    category_name = models.CharField(max_length=50)
    category_grade = models.DecimalField(max_digits=5, decimal_places=2, default=0)
    category_weightage = models.DecimalField(max_digits=5, decimal_places=2)

    # ensure uniqueness within each class
    class Meta:
        unique_together = ("associated_class", "category_name")

    # to_string function returns the name of the category
    def __str__(self):
        return self.category_name

    # function that calculates the current category using assignments and saves
    def calculate_category_grade(self):
        # get all the assignments
        assignments = self.assignment_set.all()
        # add all the assignment grades and save average
        if assignments:
            total_grade = sum(a.assignment_grade for a in assignments)
            self.category_grade = total_grade / len(assignments)
        else:
            self.category_grade = 0
        self.save()

    # override clean method to validate weightage / name
    def clean(self):
        # Check for duplicate category name within the same class
        existing_categories = Category.objects.filter(associated_class=self.associated_class, category_name=self.category_name).exclude(pk=self.pk)

        if existing_categories.exists():
            raise IntegrityError("A category with this name already exists in this class.")

        # Convert category_weightage to Decimal to ensure compatibility
        self.category_weightage = Decimal(str(self.category_weightage))

        # Calculate the total weightage including this category
        total_weightage = self.category_weightage + sum(
            category.category_weightage for category in Category.objects.filter(associated_class=self.associated_class).exclude(id=self.id)
        )
        if total_weightage > 100:
            raise IntegrityError("Total weightage of categories cannot exceed 100%.")

    # Override save method
    def save(self, *args, **kwargs):
        # call clean to validate before saving
        self.clean()
        # call the parent save method to actually save the instance
        super().save(*args, **kwargs)


# define assignment model


class Assignment(models.Model):
    associated_category = models.ForeignKey(Category, on_delete=models.CASCADE)
    assignment_name = models.CharField(max_length=50)
    assignment_grade = models.DecimalField(max_digits=5, decimal_places=2)

    # ensure uniqueness within each category
    class Meta:
        unique_together = ("associated_category", "assignment_name")

    # to_string function returns the name of the assignment
    def __str__(self):
        return self.assignment_name

    # override clean method to validate grade / name
    def clean(self):
        # Check for duplicate category name within the same class
        if Assignment.objects.filter(associated_category=self.associated_category, assignment_name=self.assignment_name).exclude(pk=self.pk).exists():
            raise ValidationError("An assignment with this name already exists in this category.")

        # Validate assignment grade range
        if self.assignment_grade > 100 or self.assignment_grade < 0:
            raise ValidationError("Assignment grade must be between 0 and 100.")

    # Override save method
    def save(self, *args, **kwargs):
        # call clean to validate before saving
        self.full_clean()
        # call the parent save method to actually save the instance
        super().save(*args, **kwargs)


# define signals to autoupdate the class and category grades as assignments are added


# class update
@receiver(post_save, sender=Category)
def update_class_grade(sender, instance, **kwargs):
    associated_class = instance.associated_class
    # update class grade when category is updated
    associated_class.calculate_class_grade()


@receiver(post_save, sender=Assignment)
def update_category_and_class_grades(sender, instance, **kwargs):
    category = instance.associated_category
    # update category grade when assignment is updated
    category.calculate_category_grade()
    # update class grade after updating category
    associated_class = category.associated_class
    associated_class.calculate_class_grade()


@receiver(post_delete, sender=Category)
def update_class_grade_on_category_delete(sender, instance, **kwargs):
    associated_class = instance.associated_class
    # recalculate class grade after category deletion
    associated_class.calculate_class_grade()


@receiver(post_delete, sender=Assignment)
def update_grades_on_assignment_delete(sender, instance, **kwargs):
    category = instance.associated_category
    # recalculate category grade after assignment deletion
    category.calculate_category_grade()
    # recalculate class grade after category grade update
    associated_class = category.associated_class
    associated_class.calculate_class_grade()
