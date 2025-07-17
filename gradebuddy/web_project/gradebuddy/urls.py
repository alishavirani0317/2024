from django.urls import path
from gradebuddy import views

urlpatterns = [
    # Class Endpoints (home page)
    path("users/<int:user_id>/classes/", views.user_classes, name="user_classes"),
    # Category Endpoints (class page)
    path("users/<int:user_id>/classes/<int:class_id>/categories/", views.get_class_page, name="get_class_page"),
    # Assignment Endpoints (categories page)
    path("users/<int:user_id>/classes/<int:class_id>/categories/<int:category_id>/assignments/", views.category_page, name="category_page"),
    # Login (POST)
    path("login/", views.user_login, name="login"),  # render login page
    # API for logging in
    path("login/api/", views.user_login, name="login_api"),
    # API for logging out
    path("logout/api/", views.user_logout, name="logout"),
    # Registration (POST)
    path("register/", views.register, name="register"),  # render registration page
    # API for registration
    path("register/api/", views.register, name="register_api"),
    # Create Class
    path("users/<int:user_id>/classes/", views.create_class, name="create_class"),
    # Create Category
    path("users/<int:user_id>/classes/<int:class_id>/categories/", views.categories_for_class, name="categories_for_class"),
    # Create Assignment
    path(
        "users/<int:user_id>/classes/<int:class_id>/categories/<int:category_id>/assignments/",
        views.category_assignments,
        name="category_assignments",
    ),
]
