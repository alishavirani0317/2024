from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

# Allow frontend to connect to backend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# Data model
class Entry(BaseModel):
    restaurant: str
    comments: str
    rating: int

@app.post("/add-entry")
def add_entry(entry: Entry):
    print(entry)
    return {"message": "Entry received!", "data": entry}
