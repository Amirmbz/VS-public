from cs50 import SQL
from flask import Flask, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Assigning the user infromation
    totals = db.execute(
        "SELECT cash FROM users WHERE id = ?",
        session["user_id"])

    # Prepering the total shares
    total_shares = 0

    # Assigning the transactions
    transactions = db.execute(
            "SELECT product_name, SUM(amount) AS shares "
            "FROM orders "
            "WHERE user_id = ? "
            "GROUP BY product_name "
            "HAVING SUM(amount) > 0 ",
            session["user_id"]
        )

    # Completing the transaction dictionary
    for transaction in transactions:

        # Looking up the price
        transaction["price"] = lookup(
            transaction["product_name"])["price"]

        # Calculating the total
        transaction["total"] = transaction["price"] * transaction["shares"]

        # Calculating the total share for totals
        total_shares += transaction["total"]

        # USD everything
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])

    # Completing the totals dictionary
    totals[0]["total"] = usd(total_shares + float(totals[0]["cash"]))
    totals[0]["shares"] = usd(total_shares)
    totals[0]["cash"] = usd(totals[0]["cash"])

    # rendering the index
    return render_template("index.html",
                           transactions=transactions,
                           totals=totals)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("stock was not submitted", 403)

        # Assining the symbol putted
        symbol = request.form.get("symbol")

        # Ensure symbol was valid
        if not lookup(symbol):
            return apology("stock was not found", 403)

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("number of shares was not submitted", 403)

        # Assining the shares putted
        shares = request.form.get("shares")

        # Ensure shares was number
        try:
            shares = float(shares)

        except ValueError:
            return apology("number of shares was not numeric", 403)

        # Ensure shares was positive
        if not shares > 0:
            return apology("number of shares was not a pisitive number", 403)

        # Estimating the value user wants to buy
        price = lookup(symbol)["price"]
        value = shares * price

        # Assigning the user infromation
        transaction = db.execute(
            "SELECT * FROM users WHERE id = ?",
            session["user_id"]
        )

        # Ensure user has enough money
        if not value < transaction[0]["cash"]:
            return apology("not enough money", 403)

        # Puchues
        db.execute(
            "INSERT INTO orders "
            "(user_id, product_name, amount, price) VALUES (?, ?, ?, ?)",
            session["user_id"],
            symbol,
            shares,
            price)

        # Increase the user cash
        transaction[0]["cash"] = transaction[0]["cash"] - value
        db.execute("UPDATE users SET cash = ? WHERE id=?",
                   (transaction[0]["cash"]),
                   session["user_id"])

        # rendering the quote with that stock
        return render_template("buy.html",
                               action=(f"{shares} of {symbol} has "
                                       f"been purcheased at {price}"),
                               value=(f"total cost {value}, "
                                      f"and {transaction[0]["cash"]} "
                                      f"in walet"))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Assigning the transactions
    transactions = db.execute(
            "SELECT * "
            "FROM orders "
            "WHERE user_id = ? "
            "ORDER BY order_id DESC",
            session["user_id"]
        )

    # Fixing transaction dictionary
    for transaction in transactions:

        # Looking up the price
        transaction["cur_price"] = usd(lookup(
            transaction["product_name"])["price"])

        # USD everything
        transaction["price"] = usd(transaction["price"])

    # rendering the index
    return render_template("history.html",
                           transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        transaction = db.execute(
            "SELECT * FROM users WHERE username = ?",
            request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(transaction) != 1 or not check_password_hash(
            transaction[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = transaction[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Ensure the quotes list is in the session
    if "quotes" not in session:
        session["quotes"] = []

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("stock was not found", 403)

        # Assining the symbol putted
        symbol = request.form.get("symbol")

        # Ensure symbol was valid
        if not lookup(symbol):
            return apology("stock was not found", 403)

        # Adding the quote requested
        for quote in session["quotes"]:
            if quote["symbol"] == lookup(symbol)["symbol"]:
                # Update the price if the symbol exists
                quote["price"] = lookup(symbol)["price"]
                session.modified = True  # Ensure the session is saved
                break
        else:
            # Append the new stock info to the session list if it's unique
            session["quotes"].append(lookup(symbol))
            session.modified = True  # Ensure the session is saved

        # rendering the quote with that stock
        return render_template("quote.html", quotes=session["quotes"])

    else:
        return render_template("quote.html", quotes=session["quotes"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure verification was submitted
        elif not request.form.get("verification"):
            return apology("must verify your password", 403)

        # Ensure that the password and verification matches
        elif request.form.get("verification") != request.form.get("password"):
            return apology("password and verification didn't match")

        # Ensure username isn't already in the database
        elif len(db.execute(
            "SELECT * FROM users WHERE username = ?",
            request.form.get("username")
        )) != 0:
            return apology("username has been taken", 403)

        # Insert the username and password into database
        transaction = db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            request.form.get("username"),
            generate_password_hash(request.form.get("password"))
        )

        # Query database for the username that we just have created
        transaction = db.execute(
            "SELECT * FROM users WHERE username = ?",
            request.form.get("username")
        )

        # Remember which user has logged in
        session["user_id"] = transaction[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("stock was not submitted", 403)

        # Assining the symbol putted
        symbol = request.form.get("symbol")

        # Query database for symbol
        transaction = db.execute(
            "SELECT SUM(amount) AS shares "
            "FROM orders "
            "WHERE product_name = ? "
            "GROUP BY product_name "
            "HAVING SUM(amount) > 0 ",
            symbol
        )

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("number of shares was not submitted", 403)

        # Assining the shares putted
        shares = request.form.get("shares")

        # Ensure shares was number
        try:
            shares = float(shares)

        except ValueError:
            return apology("number of shares was not numeric", 403)

        # Ensure shares was positive
        if not shares > 0:
            return apology("number of shares was not a pisitive number", 403)

        # Ensure share exists
        if len(transaction) != 1 or shares > transaction[0]["shares"]:
            return apology("invalid share amount", 403)

        # Estimating the value user wants to sell
        transaction[0]["price"] = lookup(symbol)["price"]
        value = shares * transaction[0]["price"]

        # Sell
        db.execute(
            "INSERT INTO orders "
            "(user_id, product_name, amount, price) VALUES (?, ?, ?, ?)",
            session["user_id"],
            symbol,
            (shares * (-1)),
            transaction[0]["price"])

        # Increase the user cash
        wallet = db.execute(
            "SELECT cash "
            "FROM users "
            "WHERE id = ? ",
            session["user_id"]
        )
        wallet[0]["cash"] += value
        db.execute("UPDATE users SET cash = ? WHERE id=?",
                   (wallet[0]["cash"]),
                   session["user_id"])

        # rendering the quote with that stock
        return render_template("sell.html",
                               action=(f"{shares} of {symbol} has "
                                       f"been purcheased at "
                                       f"{transaction[0]["price"]}"),
                               value=(f"total cost {value}, "
                                      f"and {wallet[0]["cash"]} in walet"))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")
