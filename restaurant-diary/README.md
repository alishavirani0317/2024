alisha git stuff to avoid issues

1. git pull origin main --rebase
2. make changes
3. git add .
4. git commit -m "blabla"
5. git push origin main


running backend server:
    1. cd into backend
    2. python -m venv venv
    3. source venv/bin/activate
    4. pip install fastapi uvicorn python-multipart
    5. uvicorn main:app --reload
    

running frontend server:
    npm start