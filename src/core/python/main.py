import serial
import time
import MySQLdb
import os
from dotenv import load_dotenv

# Set up database credentials
load_dotenv()
host = os.environ.get("DB_HOST")
user = os.environ.get("DB_USER")
pw = os.environ.get("DB_PASS")
db_name = os.environ.get("DB_SCHEMA")
connection = MySQLdb.connect(host, user, pw, db_name)
# DB cursor
cursor = connection.cursor()

# Initializing Arduino Serial Connection 
time.sleep(1)
device = 'COM3'     # at COM3 (port)
baud_rate = 9600
arduino = serial.Serial(device, baud_rate)