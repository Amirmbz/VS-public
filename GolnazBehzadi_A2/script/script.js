let nav = document.querySelector("nav");

window.addEventListener("scroll", function () {
  if (this.window.pageYOffset > 60) {
    nav.classList.add("scrolled");
  } else {
    nav.classList.remove("scrolled");
  }
});

function toggleMenu() {
  var nav = document.querySelector("#navList");
  nav.classList.toggle("navExpanded");
}