let imagePaths = [
  "card1",
  "card2",
  "card3",
  "card4",
  "card5",
  "card6",
  "card1",
  "card2",
  "card3",
  "card4",
  "card5",
  "card6",
];

// Function to shuffle an array
function shuffle(array) {
  for (let i = array.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
}

// Function to reveal all cards
function reveal() {
  setTimeout(() =>{
    let clicked = document.querySelectorAll(".card");
    clicked.forEach((card, index) => {
      card.classList.remove("card");
      card.classList.add(imagePaths[index]);
      
      setTimeout(() => {
        card.classList.remove(card.className);
        card.classList.add("card");
      }, 1300);
    });
  }, 1000);
}

let firstCard = null;
let secondCard = null;

function buttonsManagement() {
  let clicked = document.querySelectorAll(".card");

  clicked.forEach((card, index) => {
    card.addEventListener("click", function () {
      console.log("hi");
      if (!firstCard) {
        firstCard = card;
        card.classList.remove("card");
        card.classList.add(imagePaths[index]);
      } else if (firstCard && !secondCard && card !== firstCard) {
        secondCard = card;
        card.classList.remove("card");
        card.classList.add(imagePaths[index]);

        if (firstCard.className === secondCard.className) {
          // User guessed right
          firstCard = null;
          secondCard = null;
        } else {
          // User guessed wrong
          setTimeout(() => {
            firstCard.classList.remove(firstCard.className);
            firstCard.classList.add("card");
            secondCard.classList.remove(secondCard.className);
            secondCard.classList.add("card");
            firstCard = null;
            secondCard = null;
          }, 1000);
        }
      }
    });
  });
}

let btn = document.getElementById("btn");

btn.addEventListener("click", function () {
  shuffle(imagePaths);
  reveal();
  setTimeout(buttonsManagement, 1300);
});

let btn2 = document.getElementById("btn2");
btn2.addEventListener("click", function () {
  let clicked = document.querySelectorAll(
    ".card1, .card2, .card3, .card4, .card5, .card6"
  );

  clicked.forEach((card) => {
    card.classList.remove(card.className);
    card.classList.add("card");
  });
  firstCard = null;
  shuffle(imagePaths);
  setTimeout(reveal, 1000);
  setTimeout(buttonsManagement, 1300);
});
