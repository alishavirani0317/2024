# serializers.py

"""
Serializers (in a nutshell) convert complex data types, like Django model instances,
into formats such as JSON. This results in easier readability and rendering in web APIs.
They also handle doing the reverse process: validating and then converting input data back
into Python types for creating/updating models.

This file defines serializers for the 'Assignment', 'Category', and 'Class' models.
This allows for their data to be structe and nested properly when it comes to API responses.

- AssignmentSerializer:    Converts 'Assignment' instances to and from JSON
- CategorySerializer:      Converts 'Category' instances (including nested 'Assignment' data)
- ClassSerializer:         Converts 'Class' instances (including nested 'Category' data)
"""

from rest_framework import serializers
from gradebuddy.models import Class, Category, Assignment
from django.contrib.auth.models import User

# Assignment Serializer


class AssignmentSerializer(serializers.ModelSerializer):
    # error message values are so there are customizable error messages for the popups

    assignment_grade = serializers.DecimalField(
        max_digits=5,
        decimal_places=2,
        error_messages={
            "max_value": "Grade cannot exceed 100.",
            "min_value": "Grade cannot be less than 0.",
            "invalid": "Please enter a valid numerical grade.",
        },
    )

    associated_category = serializers.PrimaryKeyRelatedField(queryset=Category.objects.all())
    assignment_name = serializers.CharField(
        error_messages={"blank": "Assignment name cannot be empty.", "max_length": "Assignment name must be 50 characters or less."}, max_length=50
    )

    class Meta:
        model = Assignment
        fields = ["id", "assignment_name", "assignment_grade", "associated_category"]

    def validate(self, data):
        # Prepare more descriptive error messages
        error_dict = {}

        # Ensure an assignment within the category doesn't already exist
        assignment_name = data.get("assignment_name")
        associated_category = data.get("associated_category")  # use foreign key to get category

        if Assignment.objects.filter(associated_category=associated_category, assignment_name=assignment_name).exists():
            error_dict["duplicate_assignment"] = [
                f"An assignment named '{assignment_name}' already exists in this category." "Please choose a different name."
            ]

        # Validate grade range
        assignment_grade = data.get("assignment_grade")
        if assignment_grade is not None:
            if assignment_grade > 100:
                error_dict["assignment_grade"] = ["Grade cannot exceed 100. Please enter a grade between 0 and 100."]
            elif assignment_grade < 0:
                error_dict["assignment_grade"] = ["Grade cannot be negative. Please enter a grade between 0 and 100."]

        # Raise validation error if any errors exist
        if error_dict:
            raise serializers.ValidationError(error_dict)

        return data


# Category Serializer


class CategorySerializer(serializers.ModelSerializer):
    # nested assignments
    assignments = AssignmentSerializer(many=True, read_only=True, source="assignment_set")
    class_name = serializers.CharField(
        source="associated_class.class_name", read_only=True
    )  # Retrieve class_name from related Class but do not require

    class Meta:
        model = Category
        fields = ["id", "category_name", "category_grade", "category_weightage", "assignments", "class_name"]

    def validate(self, data):
        # Ensure a category within the class doen't already exist
        category_name = data.get("category_name")
        associated_class = data.get("associated_class")  # use foreign key in category to get class

        if Category.objects.filter(associated_class=associated_class, category_name=category_name).exists():
            raise serializers.ValidationError("A category with this name already exists in this class.")

        # Ensure valid weight is entered
        category_weightage = data.get("category_weightage")

        if category_weightage > 100 or category_weightage < 0:
            raise serializers.ValidationError("A category cannot have a weight greater than 100 or less than 0")

        # Calculate the total weightage of all categories in the class
        categories = Category.objects.filter(associated_class=associated_class)
        total_weightage = 0
        for c in categories:
            total_weightage += c.category_weightage
        print(total_weightage)

        # Check to make sure adding this catgeory wouldn't bring the total weightage to over 100%
        if category_weightage + total_weightage > 100:
            raise serializers.ValidationError("This category cannot be created because the total class weightage would be greater than 100%")
        return data


# Class Serializer


class ClassSerializer(serializers.ModelSerializer):
    # nested categories
    grades = CategorySerializer(many=True, read_only=True, source="categories")

    class Meta:
        model = Class
        fields = ["user", "id", "class_name", "class_grade", "percent_goal", "grades", "goal_grade", "recommendation"]
        extra_kwargs = {
            "class_name": {"required": True},
            "percent_goal": {"required": False, "default": 0},
            "class_grade": {"required": False, "default": 0},
            "goal_grade": {"required": True},
            "recommendation": {"read_only": True},  # Add recommendation as read-only
            "user": {"required": False},  # Make user field not required in input
        }

    def validate(self, data):
        # Check if a class with the same user and class name already exists
        user = data.get("user", None)
        class_name = data.get("class_name")
        if Class.objects.filter(user=user, class_name=class_name).exists():
            raise serializers.ValidationError("A class with this name already exists in your GradeBuddy.")
        # Ensure goal grade is valid
        goal_grade = data.get("goal_grade")
        if goal_grade > 100 or goal_grade < 0:
            raise serializers.ValidationError("Goal grade for a class must be between 0 and 100.")
        return data

    def create(self, validated_data):
        # Ensure class_grade has a default value if not provided
        if "class_grade" not in validated_data:
            validated_data["class_grade"] = 0
        return super().create(validated_data)


# Serializers used for data collection for views -> follows REST API


# serializer for user registration


class UserRegistrationSerializer(serializers.ModelSerializer):

    class Meta:
        model = User
        fields = ["email", "username", "password"]

    # validation to check if the username is not used
    def validate_username(self, value):
        if User.objects.filter(username=value).exists():
            raise serializers.ValidationError("This username is already taken.")
        return value

    # validation to check if the email is not used
    def validate_email(self, value):
        if User.objects.filter(email=value).exists():
            raise serializers.ValidationError("This email is already registered.")
        return value

    # function to create and return user with given info
    def create(self, validated_data):
        try:
            user = User.objects.create_user(
                email=validated_data["email"],
                username=validated_data["username"],
                password=validated_data["password"],
            )
            return user
        except Exception:
            raise serializers.ValidationError("There was an error creating the user.")


# serialzier for user login


class UserLoginSerializer(serializers.Serializer):
    username = serializers.CharField(max_length=50)  # sets limit to username length
    password = serializers.CharField(write_only=True)  # won't return password

    # validate user
    def validate(self, data):
        username = data.get("username")
        password = data.get("password")

        # if did not enter both, raise error
        if not (username and password):
            raise serializers.ValidationError("Must fill out username and password.")
        return data
