// tailwind.config.js
//run this in terminal: npx tailwindcss -i ./web_project/gradebuddy/input.css -o ./web_project/gradebuddy/output.css --watch
module.exports = {
  content: [
    './web_project/gradebuddy/login.html',
    './web_project/gradebuddy/home-page.html',
    './web_project/gradebuddy/create-account.html',
    './web_project/gradebuddy/class-page.html',
    './web_project/gradebuddy/category-page.html'
  ],
  theme: {
      extend: {
          colors: {
              cactus: '#6F927F',
              desertrose: '#A96255',
              champagne: '#F2E5D3',
              terracotta: '#B0522A',
              apricot: '#F6BC8C',
              marmalade: '#E07D45',
              beige: '#D2B295',
              lightgrey: '#CECECE',
              darkgrey: '#686868',
              lightorange: '#E5884D',
              darkorange: '#D77F48',
              darkbeige:'#B0967E',
              lightbeige:'#D2B295',
          },
          fontFamily: {
            sans: ['Freeman', 'sans-serif'],
          }
      },
  },
  plugins: [],
};
