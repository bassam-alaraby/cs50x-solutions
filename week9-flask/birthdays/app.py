import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Validate user input
        if not name or not month or not day:
            return render_template("error.html", errorMessage="INPUTS CAN NOT BE NULL")

        try:
            month = int(month)
            day = int(day)
        except ValueError:
            return render_template("error.html", errorMessage="NOT INTEGER")

        if int(month) < 1 or int(month) > 12:
            return render_template("error.html", errorMessage="INVALID MONTH")

        if int(day) < 1 or int(day) > 31:
            return render_template("error.html", errorMessage="INVALID DAY")

        # Insert data to the database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)
