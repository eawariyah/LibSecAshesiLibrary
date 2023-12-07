const firebaseConfig = {
  apiKey: "AIzaSyCzSMSm6ZHGoLGA7AToiIMHByDFz9yIIbk",
  authDomain: "ashesilibraryesp32-fd5b8.firebaseapp.com",
  databaseURL: "https://ashesilibraryesp32-fd5b8-default-rtdb.firebaseio.com",
  projectId: "ashesilibraryesp32-fd5b8",
  storageBucket: "ashesilibraryesp32-fd5b8.appspot.com",
  messagingSenderId: "185836313770",
  appId: "1:185836313770:web:ff9137a24273f2ed0844d3",
  measurementId: "G-J8PWV52BJE",
};

firebase.initializeApp(firebaseConfig);
const database = firebase.database();

function searchBook() {
  const bookRFID = document.getElementById("searchInput").value;
  if (!bookRFID) {
    alert("Please enter a valid Book RFID");
    return;
  }

  database
    .ref(`LibraryRecords/${bookRFID}`)
    .once("value")
    .then((snapshot) => {
      const bookData = snapshot.val();
      if (bookData) {
        document.getElementById("bookRFID").innerText = bookRFID;
        document.getElementById("approvalStatus").innerText = bookData.approval
          ? "Approved"
          : "Not Approved";
        document.getElementById("searchResult").style.display = "block";
      } else {
        alert("Book not found");
      }
    });
}

function checkout() {
  const studentEmail = prompt("Enter student's email address:");
  const studentID = prompt("Enter student's ID:");
  const duration = prompt("Enter duration for borrowing:");

  // Perform checkout logic here

  // Update approval status to true in the database
  database.ref(`LibraryRecords/${bookRFID}`).update({
    approval: true,
  });

  alert("Checkout successful!");
  document.getElementById("searchResult").style.display = "none";
}
