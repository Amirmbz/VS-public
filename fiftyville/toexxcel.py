import sqlite3
import pandas as pd

# Connect to the SQL database
conn = sqlite3.connect('fiftyville.db')
cursor = conn.cursor()

# Execute a query to retrieve data
query = "SELECT * FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28;"
cursor.execute(query)

# Fetch all the data
data = cursor.fetchall()

# Get the column names from the cursor
column_names = [description[0] for description in cursor.description]

# Create a DataFrame from the data
df = pd.DataFrame(data, columns=column_names)

# Write the DataFrame to an Excel file
df.to_excel('output.xlsx', index=False, engine='openpyxl')

# Close the database connection
conn.close()
