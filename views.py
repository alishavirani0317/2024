# views.py

import json

# from django.http import JsonResponse
from django.http import JsonResponse
from django.shortcuts import get_object_or_404, render
from rest_framework import status
from rest_framework.response import Response
from rest_framework.decorators import api_view
from .models import Class, Assignment, Category, User
from .serializers import ClassSerializer, AssignmentSerializer, CategorySerializer

# use serializer to collect data
from .serializers import UserRegistrationSerializer, UserLoginSerializer

# built in django user authentictaion
from django.contrib.auth import authenticate, login as auth_login
from django.contrib.auth import logout


# ======================================================================= #
# Creating Class View


@api_view(["POST"])
def create_class(request, user_id):
    try:
        # Add user_id to the request data
        data = request.data.copy()
        data["user"] = user_id

        # Create serializer with modified data
        serializer = ClassSerializer(data=data)

        if serializer.is_valid():
            # Save the class
            new_class = serializer.save()

            # Return response with success flag and class data
            return Response(
                {"success": True, "class_name": new_class.class_name, "goal_grade": new_class.goal_grade, "id": new_class.id},
                status=status.HTTP_201_CREATED,
            )

        # If validation fails, return the errors
        return Response({"success": False, "error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)

    except Exception as e:
        print(f"Error creating class: {str(e)}")
        return Response({"success": False, "error": str(e)}, status=status.HTTP_400_BAD_REQUEST)


# ---------------------------------------------------
# Retrieves all classes for a user or adds a new class (FOR HOME PAGE)


@api_view(["GET", "POST", "DELETE"])
def user_classes(request, user_id):
    if not request.user.is_authenticated or request.user.id != user_id:
        return Response({"message": "Unauthorized"}, status=status.HTTP_401_UNAUTHORIZED)

    if request.method == "GET":
        try:
            # Get the classes for the user
            classes = Class.objects.filter(user_id=user_id)

            # Serialize the classes data using ClassSerializer
            serializer = ClassSerializer(classes, many=True)

            # Pass the serialized data to the template
            return render(request, "home-page.html", {"user": request.user, "classes_json": json.dumps(serializer.data)})
        except Exception as e:
            return Response({"message": f"Error fetching classes: {str(e)}"}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "POST":
        try:
            data = request.data.copy()
            data["user"] = user_id

            serializer = ClassSerializer(data=data)

            # check if class data is valid
            if serializer.is_valid():
                new_class = serializer.save()
                return Response(
                    {"success": True, "class_name": new_class.class_name, "goal_grade": new_class.goal_grade, "id": new_class.id},
                    status=status.HTTP_201_CREATED,
                )
            else:
                return Response({"success": False, "error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
        except Exception as e:
            return Response({"success": False, "error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "DELETE":
        try:
            class_id = request.data.get("class_id")
            class_instance = Class.objects.get(id=class_id, user_id=user_id)
            class_instance.delete()
            return Response({"message": "Class deleted successfully"}, status=status.HTTP_204_NO_CONTENT)
        except Class.DoesNotExist:
            return Response({"message": "Class not found"}, status=status.HTTP_404_NOT_FOUND)
        except Exception as e:
            return Response({"message": f"Error deleting class: {str(e)}"}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


# ---------------------------------------------------
# Retrieve, add, or delete categories within a class (FOR CLASS PAGE)


@api_view(["GET", "POST"])
def categories_for_class(request, user_id, class_id):
    # Check if the requesting user is the owner of the class
    if not request.user.is_authenticated or request.user.id != int(user_id):
        return Response({"error": "You don't have permission to access this class"}, status=status.HTTP_401_UNAUTHORIZED)

    # Check if the user and class IDs are valid and match
    try:
        user = User.objects.get(id=user_id)  # Retrieve user
        class_instance = Class.objects.get(id=class_id, user=user)  # Ensures class is related to the user
    except (User.DoesNotExist, Class.DoesNotExist):
        return Response({"error": "User or class not found"}, status=404)

    if request.method == "GET":
        categories = Category.objects.filter(associated_class=class_instance)
        serializer = CategorySerializer(categories, many=True)
        # Render the class page template, passing the class and categories data

        return render(
            request,
            "class-page.html",
            {
                "user_id": user_id,
                "class_id": class_id,
                "class_name": class_instance.class_name,
                "desired_percentage": class_instance.goal_grade,
                "categories_json": json.dumps(CategorySerializer(categories, many=True).data),  # pass categories as JSON
            },
        )

    elif request.method == "POST":
        serializer = CategorySerializer(data=request.data)
        if serializer.is_valid():
            category = serializer.save(associated_class=class_instance)

            return Response(
                {"id": category.id, "category_name": category.category_name, "category_weightage": category.category_weightage}, status=201
            )
        else:
            return Response(serializer.errors, status=400)


# ---------------------------------------------------
# Accessing the Class Page


@api_view(["GET", "POST", "DELETE"])
def get_class_page(request, user_id, class_id):
    if request.method == "GET":
        if class_id:
            # Fetch the specific class and its categories
            class_instance = get_object_or_404(Class, pk=class_id, user_id=user_id)
            categories = Category.objects.filter(associated_class=class_instance)

            return render(
                request,
                "class-page.html",
                {
                    "user_id": user_id,
                    "class_id": class_id,
                    "class": class_instance,
                    "class_name": class_instance.class_name,
                    "desired_percentage": class_instance.goal_grade,
                    "categories": categories,
                    "category_grade": 0,
                },
            )
        else:
            try:
                # Get all classes for the user (home page)
                classes = Class.objects.filter(user_id=user_id)
                serializer = ClassSerializer(classes, many=True)

                return render(request, "home-page.html", {"user": request.user, "classes_json": json.dumps(serializer.data)})
            except Exception as e:
                return JsonResponse({"message": f"Error fetching classes: {str(e)}"}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "POST":
        try:
            # Post a new category for the class
            class_instance = get_object_or_404(Class, pk=class_id, user_id=user_id)

            # Prepare data to create a new category
            data = request.data.copy()
            data["user"] = user_id
            data["associated_class"] = class_instance.id
            serializer = CategorySerializer(data=data)

            # Validate and save the category
            if serializer.is_valid():
                new_category = serializer.save(associated_class=class_instance)
                return Response(
                    {
                        "success": True,
                        "category_name": new_category.category_name,
                        "weightage": new_category.category_weightage,
                        "id": new_category.id,
                    },
                    status=status.HTTP_201_CREATED,
                )
            else:
                return Response({"success": False, "error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
        except Exception as e:
            return Response({"success": False, "error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "DELETE":
        try:
            class_instance = Class.objects.get(id=class_id, user_id=user_id)
            category_id = request.data.get("category_id")
            category = Category.objects.get(id=category_id, associated_class=class_instance)
            category.delete()
            return Response({"message": "Category deleted successfully"}, status=204)
        except Category.DoesNotExist:
            return Response({"error": "Category not found"}, status=404)
        except Exception as e:
            return Response({"error": str(e)}, status=400)


# ---------------------------------------------------
# Accessing the Category Page


@api_view(["GET", "POST", "DELETE"])
def category_page(request, user_id, class_id, category_id):
    if request.method == "GET":
        try:
            # Fetch the specific class and category
            class_instance = get_object_or_404(Class, pk=class_id, user_id=user_id)
            category = get_object_or_404(Category, pk=category_id, associated_class=class_instance)

            # Get assignments for the category
            assignments = Assignment.objects.filter(associated_category=category)

            # Render the category page

            return render(
                request,
                "category-page.html",
                {
                    "user_id": user_id,
                    "class_id": class_id,
                    "category_id": category_id,
                    "category": category,
                    "class_name": class_instance.class_name,
                    "desired_percentage": class_instance.goal_grade,
                    "category_name": category.category_name,
                    "category_grade": category.category_grade,
                    "assignments": assignments,
                },
            )

        except Exception as e:
            return Response({"success": False, "error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "POST":
        try:
            # Post a new assignment for the category
            class_instance = get_object_or_404(Class, pk=class_id, user_id=user_id)
            category = get_object_or_404(Category, pk=category_id, associated_class=class_instance)

            # Prepare data to create a new assignment
            data = request.data.copy()
            data["associated_category"] = category.id
            serializer = AssignmentSerializer(data=data)

            # Validate and save the assignment
            if serializer.is_valid():
                new_assignment = serializer.save(associated_category=category)
                return Response(
                    {
                        "success": True,
                        "assignment_name": new_assignment.assignment_name,
                        "assignment_grade": new_assignment.assignment_grade,
                        "id": new_assignment.id,
                    },
                    status=status.HTTP_201_CREATED,
                )
            else:
                # function to flatten nested error messages
                def flatten_errors(errors):
                    flattened = []
                    for field, messages in errors.items():
                        if isinstance(messages, list):
                            # add ach error message for the field
                            flattened.extend([f"{field}: {message}" for message in messages])
                        elif isinstance(messages, dict):  # Handle nested errors (recursion)
                            flattened.extend(flatten_errors(messages))
                    return flattened

                # Flatten all error messages and format them as a string
                error_message = ", ".join(flatten_errors(serializer.errors))

                # Return a response with the formatted error messages
                return Response(
                    {"success": False, "error": error_message},
                    status=status.HTTP_400_BAD_REQUEST,
                )
        except Exception as e:
            return Response({"success": False, "error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

    elif request.method == "DELETE":
        try:
            class_instance = Class.objects.get(id=class_id, user_id=user_id)
            category_instance = Category.objects.get(id=category_id, associated_class=class_instance)
            assignment_id = request.data.get("assignment_id")
            assignment = Assignment.objects.get(id=assignment_id, associated_category=category_instance)
            assignment.delete()
            return Response({"message": "Assignment deleted successfully"}, status=204)
        except Assignment.DoesNotExist:
            return Response({"error": "Assignment not found"}, status=404)


# ---------------------------------------------------
# Retrieve, add, or delete assignments within a class (FOR CATEGORIES PAGE)


@api_view(["GET", "POST"])
def category_assignments(request, user_id, class_id, category_id, assignment_id=None):
    # Check if the requesting user is the owner of the class
    class_instance = get_object_or_404(Class, pk=class_id, user_id=user_id)

    if request.user.id != class_instance.user.id:

        return JsonResponse({"error": "You don't have permission to access this class"}, status=status.HTTP_401_UNAUTHORIZED)

    if request.method == "GET":
        assignments = Assignment.objects.filter(associated_category_id=category_id)
        serializer = AssignmentSerializer(assignments, many=True)
        return Response(serializer.data, status=200)

    elif request.method == "POST":
        serializer = AssignmentSerializer(data=request.data)
        if serializer.is_valid():
            assignment = serializer.save(associated_category=assignments)
            return Response({"assignment_name": assignment.assignment_name, "assignment_grade": assignment.assignment_grade}, status=201)
        else:
            return Response(serializer.errors, status=400)


# ---------------------------------------------------
# define the registration view


@api_view(["POST", "GET"])
def register(request):
    if request.method == "GET":

        # Return the registration page when the GET request is made
        return render(request, "create-account.html")

    elif request.method == "POST":
        # Handle POST request for registration
        register_serializer = UserRegistrationSerializer(data=request.data)

        # Check if incoming data is valid
        if register_serializer.is_valid():

            # Save the new user
            register_serializer.save()
            return Response({"message": "Registration successful", "status": "success"}, status=status.HTTP_201_CREATED)

        # Return error message if resgiration not successful

        # get serializer error message
        error_message = list(register_serializer.errors.values())[0][0]
        return Response({"message": f"{error_message}", "status": "error"}, status=status.HTTP_400_BAD_REQUEST)


# ---------------------------------------------------
# define the login view


@api_view(["POST", "GET"])
def user_login(request):
    if request.method == "GET":
        return render(request, "login.html")

    # Handle POST request for login
    if request.method == "POST":
        # Normal Login Process
        login_serializer = UserLoginSerializer(data=request.data)
        if login_serializer.is_valid():
            username = login_serializer.validated_data["username"]
            password = login_serializer.validated_data["password"]
            user = authenticate(request, username=username, password=password)
            if user is not None:
                auth_login(request, user)

                return Response({"message": "Login was successful!", "status": "success", "user_id": user.id}, status=status.HTTP_200_OK)
            return Response(
                {"message": "Invalid Username or Password", "status": "error"},
                status=status.HTTP_401_UNAUTHORIZED,
            )

        return Response({"message": "Invalid login data provided", "status": "error"}, status=status.HTTP_400_BAD_REQUEST)  # Not a valid request


# define the logout view


@api_view(["POST"])
def user_logout(request):
    if request.method == "POST":
        logout(request)  # Logs out the user
        return JsonResponse({"message": "Successfully logged out", "status": "success"})
    else:
        return JsonResponse({"message": "Invalid request method", "status": "error"}, status=400)
