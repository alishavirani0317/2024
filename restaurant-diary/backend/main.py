# backend/main.py
from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def home():
    return {"message": "Restaurant Diary Backend"}

