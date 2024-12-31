import os
import sys

# from urllib import response

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../..")))
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "web_project.web_project.settings")

import django

django.setup()


# from django.utils.crypto import get_random_string
from django.test import TestCase

# from django.urls import reverse
from django.contrib.auth.models import User

# from rest_framework import status
# from rest_framework.test import APIClient, APITestCase
from gradebuddy.models import Class, Category

# from gradebuddy.serializers import ClassSerializer, CategorySerializer, AssignmentSerializer
from decimal import Decimal

# from django.contrib.auth import get_user_model

from django.db import IntegrityError


class MyTestCase(TestCase):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        # Any class-level setup can be done here


# ----------------------------------------------------
# UNIT TESTS FOR CLASSES CREATION --> FRONT END TO BACK END


# class DashboardViewTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")


# #     def test_home_view_with_login(self):
# #         """Test if the home renders correctly after login."""
# #         self.client.login(username='testuser', password='password123')
# #         response = self.client.get(reverse('home'))
# #         self.assertEqual(response.status_code, 200)
# #         self.assertTemplateUsed(response, 'home.html')

# #     def test_home_view_without_login(self):
# #         """Test if unauthenticated users are redirected."""
# #         response = self.client.get(reverse('home'))
# #         self.assertEqual(response.status_code, 302)


# # ---------------------------------------------
# # Testing Serializers


# class ClassSerializerTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )

#     def test_valid_class_serializer(self):
#         """Test that the Class serializer correctly formats output."""
#         serializer = ClassSerializer(instance=self.class_instance)
#         data = serializer.data
#         self.assertEqual(data["class_name"], "Math")
#         self.assertEqual(data["class_grade"], "85.00")  # DecimalField formats as string

#     def test_invalid_class_serializer(self):
#         """Test that invalid input is caught by the serializer."""
#         invalid_data = {"class_name": "", "class_grade": "invalid"}  # Invalid data
#         serializer = ClassSerializer(data=invalid_data)
#         self.assertFalse(serializer.is_valid())
#         self.assertIn("class_grade", serializer.errors)


# class CategorySerializerTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )
#         self.category_instance = Category.objects.create(
#             associated_class=self.class_instance, category_name="Homework", category_grade=95, category_weightage=30
#         )

#     def test_valid_category_serializer(self):
#         """Test that the Category serializer correctly formats output."""
#         serializer = CategorySerializer(instance=self.category_instance)
#         data = serializer.data
#         self.assertEqual(data["category_name"], "Homework")
#         self.assertEqual(data["category_grade"], "95.00")  # Changed to decimal

#     def test_invalid_category_serializer(self):
#         """Test that invalid input is caught by the serializer."""
#         invalid_data = {"category_name": "", "category_grade": "invalid"}  # Invalid data
#         serializer = CategorySerializer(data=invalid_data)
#         self.assertFalse(serializer.is_valid())
#         self.assertIn("category_grade", serializer.errors)


# class AssignmentSerializerTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )
#         self.category_instance = Category.objects.create(
#             associated_class=self.class_instance, category_name="Homework", category_grade=95, category_weightage=30
#         )
#         self.assignment_instance = Assignment.objects.create(
#             associated_category=self.category_instance, assignment_name="Assignment 1", assignment_grade=100
#         )

#     def test_valid_assignment_serializer(self):
#         """Test that the Assignment serializer correctly formats output."""
#         serializer = AssignmentSerializer(instance=self.assignment_instance)
#         data = serializer.data
#         self.assertEqual(data["assignment_name"], "Assignment 1")
#         self.assertEqual(data["assignment_grade"], "100.00")

#     def test_invalid_assignment_serializer(self):
#         """Test that invalid input is caught by the serializer."""
#         invalid_data = {"assignment_name": "", "assignment_grade": "invalid"}  # Invalid data
#         serializer = AssignmentSerializer(data=invalid_data)
#         self.assertFalse(serializer.is_valid())
#         self.assertIn("assignment_grade", serializer.errors)


# # ---------------------------------------------
# # Testing URLs


# class URLTest(TestCase):
#     def test_user_classes_url(self):
#         """Test the user classes URL resolves to the correct view."""
#         url = reverse("user_classes", kwargs={"user_id": 1})
#         response = self.client.get(url)
#         self.assertEqual(response.status_code, 200)  # Ensure it resolves correctly

#     def test_class_categories_url(self):
#         """Test the class categories URL resolves to the correct view."""
#         url = reverse("class_categories", kwargs={"class_id": 1})
#         response = self.client.get(url)
#         self.assertEqual(response.status_code, 200)  # Ensure it resolves correctly

#     def test_category_assignments_url(self):
#         """Test the category assignments URL resolves to the correct view."""
#         url = reverse("category_assignments", kwargs={"category_id": 1})
#         response = self.client.get(url)
#         self.assertEqual(response.status_code, 200)  # Ensure it resolves correctly

#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.client = APIClient()

#     def test_get_user_classes(self):
#         """Test GET request to retrieve classes for a user."""
#         Class.objects.create(user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation="")
#         response = self.client.get(reverse("user_classes", kwargs={"user_id": self.user.id}))
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(len(response.data), 1)

#     def test_post_user_classes(self):
#         """Test POST request to create a class for a user."""
#         response = self.client.post(
#             reverse("user_classes", kwargs={"user_id": self.user.id}),
#             {"class_name": "Science", "class_grade": 90, "goal_grade": 95, "percent_goal": 90, "recommendation": ""},
#         )
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)
#         self.assertEqual(response.data["class_name"], "Science")

#     def test_post_invalid_user_classes(self):
#         """Test POST request with invalid data."""
#         response = self.client.post(
#             reverse("user_classes", kwargs={"user_id": self.user.id}),
#             {"class_name": "", "class_grade": "invalid", "goal_grade": 95, "percent_goal": 90, "recommendation": ""},
#         )
#         self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)


# class ClassCategoriesViewTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )
#         self.client = APIClient()

#     def test_get_class_categories(self):
#         """Test GET request to retrieve categories for a class."""
#         Category.objects.create(associated_class=self.class_instance, category_name="Homework", category_grade=95, category_weightage=30)
#         response = self.client.get(reverse("class_categories", kwargs={"class_id": self.class_instance.id}))
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(len(response.data), 1)

#     def test_post_class_categories(self):
#         """Test POST request to create a category for a class."""
#         response = self.client.post(
#             reverse("class_categories", kwargs={"class_id": self.class_instance.id}),
#             {"category_name": "Projects", "category_grade": 90, "category_weightage": 40},
#         )
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)
#         self.assertEqual(response.data["category_name"], "Projects")


# class CategoryAssignmentsViewTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )
#         self.category_instance = Category.objects.create(
#             associated_class=self.class_instance, category_name="Homework", category_grade=95, category_weightage=30
#         )
#         self.client = APIClient()

#     def test_login_success(self):
#         url = reverse('login_api')
#         data = {
#             "username": "testuser",
#             "password": "testpassword"
#         }
#         response = self.client.post(url, data)
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(len(response.data), 1)

#     def test_post_category_assignments(self):
#         """Test POST request to create an assignment for a category."""
#         response = self.client.post(
#             reverse("category_assignments", kwargs={"category_id": self.category_instance.id}),
#             {"assignment_name": "Assignment 2", "assignment_grade": 90},
#         )
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)
#         self.assertEqual(response.data["assignment_name"], "Assignment 2")


# # No tests for home view as per the requirement.

# # ---------------------------------------------
# # Testing Models
# # Making sure our database models behave correctly (foreign keys)z


# class ModelTest(TestCase):
#     def setUp(self):
#         self.user = User.objects.create_user(username="testuser", password="password123")
#         self.class_instance = Class.objects.create(
#             user=self.user, class_name="Math", class_grade=85, goal_grade=90, percent_goal=90, recommendation=""
#         )
#         self.category_instance = Category.objects.create(
#             associated_class=self.class_instance,
#             category_name="Homework",
#             category_grade=95,
#             category_weightage=30,
#         )
#         self.assignment_instance = Assignment.objects.create(
#             associated_category=self.category_instance,
#             assignment_name="Assignment 1",
#             assignment_grade=100,
#         )

#     def test_class_creation(self):
#         """Test that a Class instance is created successfully."""
#         self.assertEqual(self.class_instance.class_name, "Math")
#         self.assertEqual(self.class_instance.user, self.user)

#     def test_category_creation(self):
#         """Test that a Category is linked to the correct Class."""
#         self.assertEqual(self.category_instance.associated_class, self.class_instance)

#     def test_assignment_creation(self):
#         """Test that an Assignment is linked to the correct Category."""
#         self.assertEqual(self.assignment_instance.associated_category, self.category_instance)

#     def test_login_fail(self):
#         url = reverse('login_api')
#         data = {
#             "username": "testuser",
#             "password": "wrongpassword"
#         }
#         response = self.client.post(url, data)
#         self.assertEqual(response.status_code, status.HTTP_401_UNAUTHORIZED)
#         self.assertEqual(response.data['error'], "Invalid Username or Password")

#     # commented out for now until implementation is created by Alisha and Catie
#     # def test_user_classes(self):
#     #     # Log in the user
#     #     login_response = self.client.post(reverse('login_api'), {
#     #         "username": "testuser",
#     #         "password": "testpassword"
#     #     })
#     #     self.assertEqual(login_response.status_code, status.HTTP_200_OK)

#     #     # Create a class for the user
#     #     self.client.post(reverse('user_classes', kwargs={'user_id': self.user.id}), self.class_data)


# class AuthenticationTest(TestCase):
#     def setUp(self):
#         self.client = APIClient()
#         self.user = User.objects.create_user(username="testuser", email="testuser@example.com", password="password123")

#     def test_user_classes_not_authenticated(self):
#         response = self.client.get(reverse('user_classes', kwargs={'user_id': self.user.id}))
#         self.assertEqual(response.status_code, status.HTTP_302_FOUND)  # Redirect to login

#     def test_register_duplicate_user(self):
#         """Test that registration fails if the username or email already exists"""
#         # Attempt to register with the same username and email as the existing user
#         url = reverse("register")
#         data = {"email": "testuser@example.com", "username": "testuser", "password": "newpassword123"}  # Duplicate email  # Duplicate username
#         response = self.client.post(url, data)
#         # Verify response status and error message for duplicate entry
#         self.assertEqual(response.status_code, 400)


# # --------------------------------------------------------------- #

# # UNIT TESTS FOR FRONT-END AND BACK-END CONNECTION

# class UserTests(APITestCase):

#     def setUp(self):
#         # create an example user account for testing
#         self.user = User.objects.create_user(username="testuser", password="testpassword")

#     def test_register(self):
#         url = reverse("register")
#         data = {"username": "newuser", "password": "newpassword", "email": "newuser@example.com"}
#         response = self.client.post(url, data)
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)
#         self.assertEqual(response.data["message"], "Account Created Successfully")

#     def test_login_success(self):
#         url = reverse("login")
#         data = {"username": "testuser", "password": "testpassword"}
#         response = self.client.post(url, data)
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(response.data["message"], "Login was successful!")

#     def test_login_fail(self):
#         url = reverse("login")
#         data = {"username": "testuser", "password": "wrongpassword"}
#         response = self.client.post(url, data)
#         self.assertEqual(response.status_code, status.HTTP_401_UNAUTHORIZED)
#         self.assertEqual(response.data["error"], "Invalid Username or Password")

#     def test_user_is_saved_to_database(self):
#         # Generate a random username to ensure uniqueness
#         random_username = f"testuser_{get_random_string(5)}"
#         response = self.client.post(
#             reverse("register"),
#             {"username": random_username, "password": "testpassword123", "email": f"test_{get_random_string(5)}@example.com"},
#             format="json",
#         )
#         self.assertEqual(response.status_code, 201)
#         user_exists = User.objects.filter(username=random_username).exists()
#         self.assertTrue(user_exists)


# # =============================================================================

# # Connecting Front-End to Back-End Unit Test Cases

# from django.test import Client


# class UserAuthTests(TestCase):
#     def setUp(self):
#         self.client = Client()

#     def test_registration_view(self):
#         self.assertEqual(response.status_code, 200)
#         self.assertTemplateUsed(response, "create-account.html")

#     def test_login_view(self):
#         # Test the login view
#         response = self.client.get(reverse("login"))
#         self.assertEqual(response.status_code, 200)
#         self.assertTemplateUsed(response, "login.html")

#     def test_successful_registration(self):
#         """Test successful user registration"""
#         response = self.client.post(reverse("register"), {"email": "user@gmail.com", "username": "new_user", "password": "password123"})
#         # Assert that the status code is 201 (Created)
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)


# # ---------------------------------------------
# # Testing automatic updates for grade information


# class GradeUpdateTests(TestCase):

#     # sets up test cases
#     def setUp(self):
#         # creates a test user
#         self.user = User.objects.create_user(username="userabs", password="passwordabc")

#         # create a class with initial goal_grade
#         self.test_class = Class.objects.create(
#             user=self.user,
#             class_name="Class 1",
#             class_grade=0.0,  # set grade to zero since nothing added
#             goal_grade=93.0,
#             percent_goal=90,
#             recommendation="",
#         )

#     # test class updates with one category added
#     def test_class_grade_updates_with_category(self):
#         # add a category
#         Category.objects.create(associated_class=self.test_class, category_name="Category 1", category_grade=90.0, category_weightage=0.4)

#         # Refresh the class object from the database
#         self.test_class.refresh_from_db()

#         # Check if the class grade has been updated
#         self.assertEqual(self.test_class.class_grade, 90.0)  # weight of the category doesn't matetr bc there is only one

#     # test class updates with multiple categories added
#     def test_class_grade_updates_with_multiple_categories(self):
#         # add multiple categories
#         Category.objects.create(associated_class=self.test_class, category_name="Category A", category_grade=75.0, category_weightage=0.35)
#         Category.objects.create(associated_class=self.test_class, category_name="Category B", category_grade=100.0, category_weightage=0.65)

#         # Refresh the class object from the database
#         self.test_class.refresh_from_db()

#         # Check if the class grade has been updated (75*0.35 + 100*0.65 = 91.25)
#         self.assertEqual(self.test_class.class_grade, 91.25)

#     # test that category updates when one assignment is added
#     def test_category_grade_updates_with_new_assignments(self):
#         # add a category
#         category = Category.objects.create(associated_class=self.test_class, category_name="Category A", category_grade=0, category_weightage=1.0)

#         # add 4 assignments to the category
#         Assignment.objects.create(associated_category=category, assignment_name="Assignment 1", assignment_grade=83.0)
#         Assignment.objects.create(associated_category=category, assignment_name="Assignment 2", assignment_grade=99.0)
#         Assignment.objects.create(associated_category=category, assignment_name="Assignment 3", assignment_grade=63.0)
#         Assignment.objects.create(associated_category=category, assignment_name="Assignment 4", assignment_grade=87.0)

#         # refresh the category object from the database
#         category.refresh_from_db()

#         # check if the category grade has been updated with average of the assignments
#         self.assertEqual(category.category_grade, 83.0)

#     # tests that class grade updates when assignments are added (multiple levels of updating)
#     def test_class_grade_updates_with_assignment(self):
#         # Add a category
#         category_one = Category.objects.create(associated_class=self.test_class, category_name="Category 1", category_grade=0, category_weightage=0.3)
#         category_two = Category.objects.create(associated_class=self.test_class, category_name="Category 2", category_grade=0, category_weightage=0.7)

#         # Add assignments to category 1
#         Assignment.objects.create(associated_category=category_one, assignment_name="Assignment 1", assignment_grade=87.0)
#         Assignment.objects.create(associated_category=category_one, assignment_name="Assignment 2", assignment_grade=79.0)

#         # Add assignments to category 2
#         Assignment.objects.create(associated_category=category_two, assignment_name="Assignment A", assignment_grade=84.0)
#         Assignment.objects.create(associated_category=category_two, assignment_name="Assignment B", assignment_grade=92.0)

#         # Refresh the category and class objects from the database
#         category_one.refresh_from_db()
#         category_two.refresh_from_db()
#         self.test_class.refresh_from_db()


#         # Check if the class grade has been updated (24.9 + 61.6 = )
#         self.assertEqual(self.test_class.class_grade, 86.5)

#     # make sure class resets if there is no categories - should be grade of zero
#     def test_class_grade_resets_if_no_weights(self):

#         self.test_class.refresh_from_db()

#         self.assertEqual(self.test_class.class_grade, 0)


# # =================================================================
# # Testing recommendations and percent goal


class ClassGoalsTest(TestCase):

    def setUp(self):
        """Set up a user and class instance for testing."""
        # create user
        self.user = User.objects.create_user(username="testuser", password="password123")
        # create class
        self.class_instance = Class.objects.create(
            user=self.user, class_name="CS 100", goal_grade=90, class_grade=0, percent_goal=0, recommendation=""
        )

    def test_recommendation_and_percent_goal(self):
        """Test that percent_goal and recommendation are calculated correctly."""

        # make categories for class
        Category.objects.create(associated_class=self.class_instance, category_name="Homework", category_grade=80, category_weightage=0.3)
        Category.objects.create(associated_class=self.class_instance, category_name="Exams", category_grade=70, category_weightage=0.6)
        Category.objects.create(associated_class=self.class_instance, category_name="Classwork", category_grade=95, category_weightage=0.1)

        # calculate class grade
        self.class_instance.refresh_from_db()

        # calculate expected percent_goal
        expected_class_grade = (80 * 0.3 + 70 * 0.6 + 95 * 0.1) / (0.3 + 0.6 + 0.1)  # Weighted average
        expected_percent_goal = expected_class_grade / 90 * 100  # Goal grade is 90
        expected_percent_goal = Decimal(expected_percent_goal)

        # set expected recommendation based on percent_goal
        if expected_percent_goal > 100:
            expected_recommendation = "Awesome job, you reached your goal!! You should be really proud :)"
        elif expected_percent_goal > 85:
            expected_recommendation = "Keep up the great work! You are almost at your goal grade:)"
        elif expected_percent_goal > 60:
            expected_recommendation = "Try to improve scores in the Exams assignments. This will help get you closer to your goal"
        else:
            expected_recommendation = "Try focusing on either Exams or Exams as these are impacting your grade the most."

        # check that matches
        self.assertAlmostEqual(self.class_instance.percent_goal, expected_percent_goal, places=2)
        self.assertEqual(self.class_instance.recommendation, expected_recommendation)

    def test_recommendation_and_percent_goal_no_categories(self):
        """Test that percent_goal and recommendation handle no categories correctly."""
        self.class_instance.categories.all().delete()  # Remove all categories

        # Cclculate grade with no categories
        self.class_instance.calculate_class_grade()

        # check that matches
        self.assertEqual(self.class_instance.percent_goal, 0)
        self.assertEqual(
            self.class_instance.recommendation, "Make sure to add in some class categories and assignments to take advantage of GradeBuddy!"
        )


# -----------------------------------------
#  Kori Database Implementation Tests


# class ClassCreationAndLoginTest(TestCase):
#     def setUp(self):

#         # Your setup code, like creating test user, etc.
#         self.user = get_user_model().objects.create_user(username="testuser", password="password123")

#     def test_create_class_and_logout_login(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')

#         # Post to create a new class
#         response = self.client.post(reverse('create_class', kwargs={'user_id': self.user.id}), {
#             'class_name': 'Math 101',
#             'percent_goal': 85
#         })

#         # Verify the class creation was successful
#         self.assertEqual(response.status_code, status.HTTP_201_CREATED)

#         # Log the user out
#         self.client.logout()

#         # Log back in
#         self.client.login(username='testuser', password='password123')

#         # Now visit the classes page
#         response = self.client.get(reverse('user_classes', kwargs={'user_id': self.user.id}))

#         # Check that the class appears in the response content
#         self.assertContains(response, 'Math 101')  # Check that the class name is in the HTML content


# -----------------------------------------
#  Kori Class Page Creation & Implementation Tests


# from django.urls import reverse
# from django.contrib.auth import get_user_model


# class ClassPageAndCategoryTest2(TestCase):
#     def setUp(self):
#         # Create a test user
#         self.user = get_user_model().objects.create_user(username="testuser", password="password123")

#         # Create a test class
#         self.test_class = Class.objects.create(
#             user=self.user, class_name="Test Class 101", class_grade=0, percent_goal=0, goal_grade=85, recommendation=""
#         )

#     def test_access_class_page(self):
#         # Log the user in
#         self.client.login(username="testuser", password="password123")

#         # Access the class page
#         response = self.client.get(reverse("class_page", kwargs={"user_id": self.user.id, "class_id": self.test_class.id}))

# class ClassPageAndCategoryTest(TestCase):
#     def setUp(self):
#         # Create a test user
#         self.user = get_user_model().objects.create_user(
#             username="testuser",
#             password="password123"
#         )

#         # Create a test class
#         self.test_class = Class.objects.create(
#             user=self.user,
#             class_name="Test Class 101",
#             class_grade=0,
#             percent_goal=0,
#             goal_grade=85,
#             recommendation=""
#         )

#     def test_access_class_page(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')

#         # Access the class page
#         response = self.client.get(
#             reverse('class_page',
#                     kwargs={'user_id': self.user.id, 'class_id': self.test_class.id})
#         )

#         # Verify successful access
#         self.assertEqual(response.status_code, status.HTTP_200_OK)


#         # Check that class information is in the response
#         self.assertContains(response, "Test Class 101")
#         self.assertContains(response, "85")  # goal_grade

#     def test_access_nonexistent_class_page(self):
#         # Log the user in
#         self.client.login(username="testuser", password="password123")

#         # Try to access a non-existent class
#         response = self.client.get(reverse("class_page", kwargs={"user_id": self.user.id, "class_id": 999}))

#         # Check that class information is in the response
#         self.assertContains(response, 'Test Class 101')
#         self.assertContains(response, '85')  # goal_grade

#     def test_access_nonexistent_class_page(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')

#         # Try to access a non-existent class
#         response = self.client.get(
#             reverse('class_page',
#                     kwargs={'user_id': self.user.id, 'class_id': 999})
#         )

#         # Verify 404 response
#         self.assertEqual(response.status_code, status.HTTP_404_NOT_FOUND)

#     def test_create_category(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')
#         # Create a test user
#         self.user = User.objects.create_user(username="testuser", password="password123")


#     def test_create_invalid_category(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')

#         # Create invalid category data (missing weightage)
#         invalid_category_data = {
#             'category_name': 'Homework'
#         }

#         # Post request to create category
#         response = self.client.post(
#             reverse('category_assignments',
#                     kwargs={'user_id': self.user.id, 'class_id': self.test_class.id}),
#             invalid_category_data
#         )

#     def test_create_category(self):
#         # Log the user in
#         self.client.login(username="testuser", password="password123")
#         # Verify bad request response
#         self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

#     def test_get_categories(self):
#         # Log the user in
#         self.client.login(username='testuser', password='password123')

#         # Create a test category
#         Category.objects.create(
#             associated_class=self.test_class,
#             category_name='Homework',
#             category_weightage=30.00
#         )

#         # Get request for categories
#         response = self.client.get(
#             reverse('category_assignments',
#                     kwargs={'user_id': self.user.id, 'class_id': self.test_class.id})
#         )

#         # Verify successful retrieval
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(len(response.data), 1)
#         self.assertEqual(response.data[0]['category_name'], 'Homework')

# --------------------------------------------------------
# Test cases for validation checks


class ValidationTests(TestCase):
    def setUp(self):
        # Create a test user
        self.user = User.objects.create_user(username="testuser", password="password123")

    #     def test_duplicate_class_name(self):
    #         # Create the first class
    #         Class.objects.create(
    #             user=self.user,
    #             class_name="Math 101",
    #             goal_grade=90,
    #             class_grade=0,
    #             percent_goal=0
    #         )
    #         # Attempt to create a duplicate class with the same name
    #         try:
    #             duplicate_class = Class(
    #                 user=self.user,
    #                 class_name="Math 101",  # Duplicate name
    #                 goal_grade=85,
    #                 class_grade=0,
    #                 percent_goal=0
    #             )

    #             duplicate_class.save()  # Attempt to save the duplicate
    #         except IntegrityError:
    #             pass  # Expected exception for duplicate name, test passes
    #         else:
    #             self.fail("Error not raised for duplicate class.")

    #     def test_duplicate_category_name(self):
    #         # Create a class first
    #         test_class = Class.objects.create(
    #             user=self.user,
    #             class_name="Science",
    #             goal_grade=85,
    #             class_grade=0,
    #             percent_goal=0
    #         )
    #         # Create the first category in science class
    #         Category.objects.create(
    #             associated_class=test_class,
    #             category_name="Homework",
    #             category_grade=0,
    #             category_weightage=30
    #         )
    #         # Attempt to create a duplicate category with the same name
    #         try:
    #             duplicate_category = Category(
    #                 associated_class=test_class,
    #                 category_name="Homework",  # Duplicate name
    #                 category_grade=0,
    #                 category_weightage=30
    #             )
    #             duplicate_category.save()  # Attempt to save the duplicate
    #         except IntegrityError:
    #             pass  # Expected exception for duplicate category name, test passes
    #         else:
    #             self.fail("Error not raised for duplicate category.")

    #     def test_duplicate_assignment_name(self):
    #         # Create a class and category first
    #         test_class = Class.objects.create(
    #             user=self.user,
    #             class_name="History",
    #             goal_grade=88,
    #             class_grade=0,
    #             percent_goal=0
    #         )
    #         test_category = Category.objects.create(
    #             associated_class=test_class,
    #             category_name="Quizzes",
    #             category_grade=0,
    #             category_weightage=20
    #         )
    #         # Create the first assignment
    #         Assignment.objects.create(
    #             associated_category=test_category,
    #             assignment_name="Quiz 1",
    #             assignment_grade=85
    #         )
    #         # Attempt to create a duplicate assignment with the same name
    #         try:
    #             duplicate_assignment = Assignment(
    #                 associated_category=test_category,
    #                 assignment_name="Quiz 1",  # Duplicate name
    #                 assignment_grade=90
    #             )
    #             duplicate_assignment.save()  # Attempt to save the duplicate
    #         except IntegrityError:
    #             pass  # Expected exception for duplicate assignment name, test passes
    #         else:
    #             self.fail("Error not raised for duplicate assignment.")

    #     def test_category_weightage_not_over_100(self):

    #         test_class = Class.objects.create(
    #             user=self.user,
    #             class_name="History",
    #             goal_grade=88,
    #             class_grade=0,
    #             percent_goal=0
    #         )

    #         # Create a category with 60% weightage
    #         Category.objects.create(
    #             associated_class=test_class,
    #             category_name="Homework",
    #             category_grade=0,
    #             category_weightage=60
    #         )

    #         # create a new category that will cause total weightage to exceed 100%
    #         category2 = Category(
    #             associated_class=test_class,
    #             category_name="Quizzes",
    #             category_grade=0,
    #             category_weightage=50  # this would make the total weightage 110%
    #         )

    #         # Call the clean method directly on category2 to test the validation
    #         with self.assertRaises(IntegrityError):
    #             category2.clean()  # This will trigger the validation logic in the clean method

    def test_independent_classes_can_have_same_category_name(self):
        # Create two independent classes
        class1 = Class.objects.create(user=self.user, class_name="Math 101", goal_grade=90, class_grade=0, percent_goal=0)
        class2 = Class.objects.create(user=self.user, class_name="Science 101", goal_grade=85, class_grade=0, percent_goal=0)

        # Create a category in the first class
        Category.objects.create(associated_class=class1, category_name="Homework", category_grade=0, category_weightage=30)

        # Create a category with the same name in the second class (independent classes can have the same category name)
        try:
            Category.objects.create(
                associated_class=class2, category_name="Homework", category_grade=0, category_weightage=20  # same name, but in a different class
            )
        except IntegrityError:
            self.fail("IntegrityError raised for independent classes with same category name.")


# --------------------------------------------------------
# Test Cases for Assignment Implementation


# class AssignmentPageAndCreationTest(TestCase):
#     def setUp(self):
#         # Create a test user
#         self.user = get_user_model().objects.create_user(username="testuser", password="password123")

#         # Create a test class
#         self.test_class = Class.objects.create(
#             user=self.user, class_name="Test Class 101", class_grade=0, percent_goal=0, goal_grade=85, recommendation=""
#         )

#         # Create a test category
#         self.test_category = Category.objects.create(
#             associated_class=self.test_class, category_name="Homework", category_weightage=20, category_grade=0
#         )

#     def test_create_assignment_success(self):
#         # Make a POST request to create an assignment
#         response = self.client.post(
#             f"/users/{self.user.id}/classes/{self.test_class.id}/categories/{self.test_category.id}/assignments/",
#             {"assignment_name": "Math Homework 1", "assignment_grade": 95},)

#     def test_get_categories(self):
#         # Log the user in
#         self.client.login(username="testuser", password="password123")

#         # Create a test category
#         Category.objects.create(associated_class=self.test_class, category_name="Homework", category_weightage=30.00)

#         # Get request for categories
#         response = self.client.get(reverse("category_assignments", kwargs={"user_id": self.user.id, "class_id": self.test_class.id}))

#         # Verify successful retrieval
#         self.assertEqual(response.status_code, status.HTTP_200_OK)
#         self.assertEqual(len(response.data), 1)
#         self.assertEqual(response.data[0]["category_name"], "Homework")

#         # Call the clean method directly on category2 to test the validation
#         with self.assertRaises(IntegrityError):
#             category2.clean()  # This will trigger the validation logic in the clean method

#     def test_independent_classes_can_have_same_category_name(self):
#         # Create two independent classes
#         class1 = Class.objects.create(
#             user=self.user,
#             class_name="Math 101",
#             goal_grade=90,
#             class_grade=0,
#             percent_goal=0
#         )
#         class2 = Class.objects.create(
#             user=self.user,
#             class_name="Science 101",
#             goal_grade=85,
#             class_grade=0,
#             percent_goal=0
#         )

#         # Assert the response status is 201 (Created)
#         self.assertEqual(response.status_code, 201)

#         # Assert the assignment was created
#         self.assertEqual(Assignment.objects.count(), 1)
#         assignment = Assignment.objects.first()
#         self.assertEqual(assignment.assignment_name, "Math Homework 1")
#         self.assertEqual(assignment.assignment_grade, 95)

#     def test_create_duplicate_assignment(self):
#         # Create an assignment
#         Assignment.objects.create(associated_category=self.test_category, assignment_name="Math Homework 1", assignment_grade=95)

#         # Attempt to create a duplicate assignment
#         response = self.client.post(
#             f"/users/{self.user.id}/classes/{self.test_class.id}/categories/{self.test_category.id}/assignments/",
#             {"assignment_name": "Math Homework 1", "assignment_grade": 85},
#         )

#         # Assert the response status is 400 (Bad Request)
#         self.assertEqual(response.status_code, 400)

#         # Assert no duplicate assignment was created
#         self.assertEqual(Assignment.objects.count(), 1)
