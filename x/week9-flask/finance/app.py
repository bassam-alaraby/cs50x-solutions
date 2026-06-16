import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
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

    # Select stocks & shares from the database
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0",
        session["user_id"]
    )

    # Find the current price & Calculate the total value
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["current_price"] = quote["price"]
        stock["total_value"] = stock["current_price"] * stock["total_shares"]

    # User Cash
    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    user_cash = rows[0]["cash"]

    # Grand Total
    grand_total = 0
    for stock in stocks:
        grand_total += stock["total_value"]

    grand_total += user_cash

    return render_template("index.html", stocks=stocks, user_cash=user_cash, grand_total=grand_total)


@app.route("/add_cash", methods=["POST"])
@login_required
def add_cash():
    amount = request.form.get("amount")

    if not amount:
        return apology("must provide amount")

    try:
        amount = int(amount)
    except ValueError:
        return apology("amount must be a positive integer")

    if amount < 1:
        return apology("amount must be a positive integer")

    db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amount, session["user_id"])
    return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate user inputs
        if not symbol:
            return apology("must provide symbol")

        quoted_symbol = lookup(symbol)
        if quoted_symbol == None:
            return apology("incorrect symbol")

        if not shares:
            return apology("must provide shares")

        try:
            shares = int(shares)
        except ValueError:
            return apology("shares must be a positive integer")

        if shares < 1:
            return apology("shares must be a positive integer")

        # Check user info
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Chech if the user have enough cash for shares
        user_cash = rows[0]["cash"]
        shares_price = quoted_symbol["price"] * shares

        if user_cash < shares_price:
            return apology("you don't have enough cash")

        else:
            # Insert the transaction into the database & Update user cash
            db.execute(
                "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                session["user_id"],
                symbol,
                shares,
                quoted_symbol["price"]
            )

            updated_cash = user_cash - shares_price
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?",
                updated_cash,
                session["user_id"]
            )

            return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Changing the current password"""

    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Validate user inputs
        if not current_password:
            return apology("must provide your current password")

        if not new_password:
            return apology("must provide a new password")

        if not confirmation:
            return apology("you must confirm your password")

        if new_password != confirmation:
            return apology("passwords must match")

        # Check passwords
        rows = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])
        hash_password = rows[0]["hash"]

        if not check_password_hash(hash_password, current_password):
            return apology("current password is incorrect")

        if check_password_hash(hash_password, new_password):
            return apology("new password must be different from current password")

        # Update the password
        db.execute(
            "UPDATE users SET hash = ? WHERE id = ?",
            generate_password_hash(new_password),
            session["user_id"]
        )

        return redirect("/")

    else:
        return render_template("change_password.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute(
        "SELECT symbol, shares, price, timestamp FROM transactions WHERE user_id = ? ORDER BY timestamp DESC",
        session["user_id"]
    )

    for transaction in transactions:
        if transaction["shares"] > 0:
            transaction["state"] = "Buy"
        else:
            transaction["shares"] *= -1
            transaction["state"] = "Sell"

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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

    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol")

        result = lookup(symbol)
        if result == None:
            return apology("incorrect symbol")

        return render_template("quoted.html", data=result)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Validate user input
        if not username:
            return apology("must provide username")

        if not password:
            return apology("must provide passwords")

        if not confirmation:
            return apology("you must confirm your password")

        if password != confirmation:
            return apology("passwords must match")

        # Insert the user to the database
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)",
                username,
                generate_password_hash(password)
            )
        except ValueError:
            return apology("username already exist")

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Log the user in
        session["user_id"] = rows[0]["id"]
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate user inputs
        if not symbol:
            return apology("must provide symbol")

        quoted_symbol = lookup(symbol)
        if quoted_symbol == None:
            return apology("incorrect symbol")

        if not shares:
            return apology("must provide shares")

        try:
            shares = int(shares)
        except ValueError:
            return apology("shares must be a positive integer")

        if shares < 1:
            return apology("shares must be a positive integer")

        # check for the stocks and shares
        owned_stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0",
            session["user_id"]
        )

        found = False
        for stock in owned_stocks:
            if stock["symbol"] == symbol:
                found = True
        if not found:
            return apology("you don't own this stock")

        rows = db.execute(
            "SELECT SUM(shares) as total_shares FROM transactions WHERE user_id = ? AND symbol = ?",
            session["user_id"], symbol
        )

        owned_shares = rows[0]["total_shares"]

        if shares > owned_shares:
            return apology("you don't have enough shares")

        else:
            # Insert the transaction into the database & Update user cash

            rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            user_cash = rows[0]["cash"]
            shares_price = quoted_symbol["price"] * shares

            db.execute(
                "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                session["user_id"],
                symbol,
                -shares,
                quoted_symbol["price"]
            )

            updated_cash = user_cash + shares_price
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?",
                updated_cash,
                session["user_id"]
            )

            return redirect("/")

    else:
        stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0",
            session["user_id"]
        )

        return render_template("sell.html", stocks=stocks)
