import sqlite3
from flask import Flask, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Function to get a database connection
def get_db_connection():
    conn = sqlite3.connect("birthdays.db")
    conn.row_factory = sqlite3.Row
    return conn


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

        # Access form data
        name = request.form.get("name")
        if not name:
            return redirect("/")

        month = request.form.get("month")
        if not month:
            return redirect("/")
        try:
            month = int(month)
        except ValueError:
            return redirect("/")
        if month < 1 or month > 12:
            return redirect("/")

        day = request.form.get("day")
        if not day:
            return redirect("/")
        try:
            day = int(day)
        except ValueError:
            return redirect("/")
        if day < 1 or day > 31:
            return redirect("/")

        # Check if the data is already existed
        conn = get_db_connection()
        existing = conn.execute("SELECT * FROM birthdays WHERE name = ?",
                                (name,)).fetchall()
        if existing:
            return redirect("/")

        # Insert data into database
        conn.execute(
            "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)",
            (name, month, day))
        conn.commit()
        conn.close()
        return redirect("/")

    else:

        conn = get_db_connection()
        birthdays = conn.execute("SELECT * FROM birthdays").fetchall()
        conn.close()

        return render_template("index.html", birthdays=birthdays)


if __name__ == "__main__":
    app.run(debug=True)
