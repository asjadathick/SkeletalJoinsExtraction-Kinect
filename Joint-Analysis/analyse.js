const fs = require('fs');

function readFile(fileName) {
    return new Promise((resolve, reject) => {

        fs.readFile(fileName, (error, data) => {

            if (error) {
                // Will end up in catch of promise
                return reject(error);
            } else {
                // Get the data and parse in one line
                return resolve(JSON.parse(data.toString()));
            }

        })
    })
}

function distBetweenPoints(p1, p2){
    let tmp = p1.split(',');
    let p1x, p1y, p2x, p2y;
    [p1x, p1y] = tmp;

    tmp = p2.split(',');
    [p2x, p2y] = tmp;

    return Math.sqrt(((p2x - p1x) * (p2x - p1x)) + ((p2y - p1y) * (p2y - p1y)));
}

function calcPersonJoinLength(data) {
    for (let entry of data) {
        entry.height = (distBetweenPoints(entry.SpineBase, entry.SpineMid));
    }
}

// ASYNC/AWAIT MAIN
(async () => {
    try {
        const fileName = "stand-ext.txt";
        let data = await readFile(fileName);
        calcPersonJoinLength(data);

        //console.log(data);
    } catch(e) {
        console.log(e);
    }
})();


// async function analyzeData3Times(data) {
//     for (let i = 0; i < 3; i++) {
//         let data = await readFile(filename);
//     }
//     return something;
// }
