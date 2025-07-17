import './App.css';
import MovieCard from './components /MovieCard';
//where react stuff goes 

//component start with capital letter
function App() {
  return (
    //return just one root element per level
    // fragment -> empty tag <>
    <>
      <MovieCard movie={{title: "Alisha's Film", release_date:"2024"}}/>
      <MovieCard movie={{title: "Dih's Film", release_date:"2024"}}/>

    </>
  );
}
// add prop : set of curly braces



export default App;
