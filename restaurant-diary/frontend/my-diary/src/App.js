import { useState } from "react";

function App() {
  const [restaurant, setRestaurant] = useState("");
  const [comments, setComments] = useState("");
  const [rating, setRating] = useState(5);
  const [message, setMessage] = useState("");

  const handleSubmit = async (e) => {
    e.preventDefault();
    const entry = { restaurant, comments, rating };
    const res = await fetch("http://localhost:8000/add-entry", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(entry),
    });
    const data = await res.json();
    setMessage(data.message);
    setRestaurant("");
    setComments("");
    setRating(5);
  };

  return (
    <div style={{ padding: "2rem", fontFamily: "Arial" }}>
      <h1>My Restaurant Diary</h1>
      <form onSubmit={handleSubmit}>
        <input
          placeholder="Restaurant Name"
          value={restaurant}
          onChange={(e) => setRestaurant(e.target.value)}
        /><br /><br />
        <textarea
          placeholder="What did you think?"
          value={comments}
          onChange={(e) => setComments(e.target.value)}
        /><br /><br />
        <input
          type="number"
          min="1"
          max="5"
          value={rating}
          onChange={(e) => setRating(parseInt(e.target.value))}
        /><br /><br />
        <button type="submit">Save Entry</button>
      </form>
      {message && <p style={{ color: "green" }}>{message}</p>}
    </div>
  );
}

export default App;
