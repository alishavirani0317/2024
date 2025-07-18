import MovieCard from "../components /MovieCard"

//contains entire user interface for home page
function Home () {
    //array of diff movies and renders them dynamically 

    const movies = [
        {id: 1, title: "John Wick", release_date: "2020" },
        {id: 2, title: "Terminator", release_date: "1999" },
        {id: 3, title: "The Matrix", release_date: "1998" },
    ]


    //display movies in rows?
    return <div className="home">
        <div className="movies-grid">
            {movies.map((movie) => (
                <MovieCard movie={movie} key={movie.id}/>
            ))}
        </div>
    </div>
}