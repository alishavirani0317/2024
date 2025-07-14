from flask import Flask

# instance of flask object
app = Flask (__name__)

#map url route to this function
@app.route("/")

def home():
    return "Welcome to Restaurant Diary!"