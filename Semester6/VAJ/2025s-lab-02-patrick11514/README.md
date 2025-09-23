[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QNE3naG4)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=18438264)
## TASKS

## Exercise for internal modules
1. MOVE ALL FUNCTIONS and array from `functions.js` into separate files in folders `utils` and `data` - split it by comments above functions
    * function for converting color in hex format -> move into `utils/colors.js` 
    * move math functions into `utils/math.js`
    * same for another types of functions
2. You should have only one implementation of every function. If you need specific function in different file, use export/import
3. Don't forgot to import the moved functiosn again in the file functions.js
TIP: When you run `node functions.js` it should give you the same output as for `node test-functions.js`

## Exercise for 3rd party mobules and packages
1. Install [axios](https://www.npmjs.com/package/axios) package as dependency - you can find instruction on website or in [presentation](https://docs.google.com/presentation/d/1JrufeLjxbBKKjWqnj1l32hcouQ3Hw-8rMIn_zzCTF7U/edit#slide=id.gef8b43e128_0_14).
2. Install [prettier](https://www.npmjs.com/package/prettier) as dev dependency package - [presentation](https://docs.google.com/presentation/d/1JrufeLjxbBKKjWqnj1l32hcouQ3Hw-8rMIn_zzCTF7U/edit#slide=id.gec017bc9e0_0_0).
3. Create new script in package.json to run prettier for every js file in project. So this command `npm run prettier` has to work and it will fix all your files in project. [Custom scripts](https://docs.google.com/presentation/d/1JrufeLjxbBKKjWqnj1l32hcouQ3Hw-8rMIn_zzCTF7U/edit#slide=id.gef8b43e128_0_3) - the command for prettier to format your files is `prettier --write .` So when you run `npm run prettier` it should execute `prettier --write .`
5. Do all tasks in `task-read.js`
6. Do all tasks in `task-axios.js` 
7. Task `task-bonus.js` is a bonus task for extra point. 
