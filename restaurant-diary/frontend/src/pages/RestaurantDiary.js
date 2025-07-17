import React, { useState, useEffect } from 'react';
import axios from 'axios';

function RestaurantDiary() {
  const [entries, setEntries] = useState([]);
  const [form, setForm] = useState({
    restaurant_name: '',
    rating: '',
    notes: ''
  });

  const fetchEntries = () => {
    axios.get('/api/entries').then(res => setEntries(res.data));
  };

  useEffect(() => {
    fetchEntries();
  }, []);

  const handleSubmit = (e) => {
    e.preventDefault();
    axios.post('/api/entries', form)
      .then(() => {
        setForm({ restaurant_name: '', rating: '', notes: '' });
        fetchEntries();
      });
  };

  const handleDelete = (id) => {
    axios.delete(`/api/entries/${id}`).then(fetchEntries);
  };

  return (
    <div className="max-w-2xl mx-auto p-4">
      <h1 className="text-3xl font-bold mb-4">Restaurant Diary</h1>

      <form onSubmit={handleSubmit} className="space-y-4 bg-white p-4 shadow rounded">
        <input
          type="text"
          placeholder="Restaurant Name"
          className="w-full border p-2"
          value={form.restaurant_name}
          onChange={(e) => setForm({ ...form, restaurant_name: e.target.value })}
          required
        />
        <input
          type="number"
          placeholder="Rating (1-5)"
          className="w-full border p-2"
          value={form.rating}
          onChange={(e) => setForm({ ...form, rating: e.target.value })}
          min="1"
          max="5"
          required
        />
        <textarea
          placeholder="Notes"
          className="w-full border p-2"
          value={form.notes}
          onChange={(e) => setForm({ ...form, notes: e.target.value })}
        />
        <button className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700" type="submit">
          Add Entry
        </button>
      </form>

      <ul className="mt-8 space-y-4">
        {entries.map(entry => (
          <li key={entry.id} className="bg-gray-100 p-4 rounded shadow">
            <div className="flex justify-between">
              <h2 className="text-xl font-semibold">{entry.restaurant_name}</h2>
              <button className="text-red-500" onClick={() => handleDelete(entry.id)}>Delete</button>
            </div>
            <p>Rating: {entry.rating}/5</p>
            <p>{entry.notes}</p>
            <p className="text-sm text-gray-500">Visited on: {entry.visit_date}</p>
          </li>
        ))}
      </ul>
    </div>
  );
}

export default RestaurantDiary;
