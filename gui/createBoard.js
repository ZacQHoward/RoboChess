let rowCount = 0;
const rowMax = 8;
let colCount = 0;
const colMax = 8;
const container = document.getElementById("boardContainer");
let LoD = true;
for (rowCount; rowCount<rowMax; rowCount++){
    const rowContainer = document.getElementById(`rowContainer${rowCount}`);
    for(colCount = 0; colCount<colMax; colCount++){
        const span = document.createElement("span");
        if(LoD){
            span.className = "board-light";
            LoD = false;
        } else {
            span.className = "board-dark";
            LoD = true;
        }
        switch(colCount){
            case 0:
                span.id = `A${8-rowCount}`;
                span.textContent = `A${8-rowCount}`;
                break;
            case 1:
                span.id = `B${8-rowCount}`;
                span.textContent = `B${8-rowCount}`;
                break;
            case 2:
                span.id = `C${8-rowCount}`;
                span.textContent = `C${8-rowCount}`;
                break;
            case 3:
                span.id = `D${8-rowCount}`;
                span.textContent = `D${8-rowCount}`;
                break;
            case 4:
                span.id = `E${8-rowCount}`;
                span.textContent = `E${8-rowCount}`;
                break;
            case 5:
                span.id = `F${8-rowCount}`;
                span.textContent = `F${8-rowCount}`;
                break;
            case 6:
                span.id = `G${8-rowCount}`;
                span.textContent = `G${8-rowCount}`;
                break;
            case 7:
                span.id = `H${8-rowCount}`;
                span.textContent = `H${8-rowCount}`;
                break;
        }
        rowContainer.appendChild(span); 
    }
    LoD = LoD ? false : true;
}

function resetBoard() {
    const startingPositions = {
        "WRRook": ['A1', "images/wR.svg"],
        "WRKnight": ['B1', "images/wN.svg"],
        "WRBishop": ['C1', "images/wB.svg"],
        "WQueen": ['D1', "images/wQ.svg"],
        "WKing": ['E1', "images/wK.svg"],
        "WLBishop": ['F1', "images/wB.svg"],
        "WLKnight": ['G1', "images/wN.svg"],
        "WLRook": ['H1', "images/wR.svg"],
        "WAPawn": ['A2', "images/wP.svg"],
        "WBPawn": ['B2', "images/wP.svg"],
        "WCPawn": ['C2', "images/wP.svg"],
        "WDPawn": ['D2', "images/wP.svg"],
        "WEPawn": ['E2', "images/wP.svg"],
        "WFPawn": ['F2', "images/wP.svg"],
        "WGPawn": ['G2', "images/wP.svg"],
        "WHPawn": ['H2', "images/wP.svg"],
        "BRRook": ['A8', "images/bR.svg"],
        "BRKnight": ['B8', "images/bN.svg"],
        "BRBishop": ['C8', "images/bB.svg"],
        "BQueen": ['D8', "images/bQ.svg"],
        "BKing": ['E8', "images/bK.svg"],
        "BLBishop": ['F8', "images/bB.svg"],
        "BLKnight": ['G8', "images/bN.svg"],
        "BLRook": ['H8', "images/bR.svg"],
        "BAPawn": ['A7', "images/bP.svg"],
        "BBPawn": ['B7', "images/bP.svg"],
        "BCPawn": ['C7', "images/bP.svg"],
        "BDPawn": ['D7', "images/bP.svg"],
        "BEPawn": ['E7', "images/bP.svg"],
        "BFPawn": ['F7', "images/bP.svg"],
        "BGPawn": ['G7', "images/bP.svg"],
        "BHPawn": ['H7', "images/bP.svg"],
    };
    for (const entry of Object.entries(startingPositions)) {
        const img =document.createElement('img');
        img.setAttribute('src', entry[1][1]);
        img.setAttribute('class', "piece");
        document.getElementById(entry[1][0]).appendChild(img);
    }
}
