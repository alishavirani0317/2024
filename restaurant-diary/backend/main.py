from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

# Create FastAPI app instance
app = FastAPI()

# Allow frontend to connect to backend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Allow all origins during development
    allow_methods=["*"],
    allow_headers=["*"],
)

# Data model
class Entry(BaseModel):
    restaurant: str
    comments: str
    rating: int

# API route to receive form data
@app.post("/add-entry")
def add_entry(entry: Entry):
    print(entry)
    return {"message": "Entry received!", "data": entry}

# Optional: simple test route
@app.get("/")
def home():
    return {"message": "Restaurant Diary Backend is working"}
