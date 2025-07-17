import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import RestaurantDiary from './pages/RestaurantDiary';

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<RestaurantDiary />} />
      </Routes>
    </Router>
  );
}

export default App;
