import os
import sys
import random
import string
import sqlite3
import argparse

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate database files in a directory.")
    parser.add_argument("directory", help="Path to the directory.")
    parser.add_argument("-n", "--num-dbs", dest='number_dbs', type=int, default=10, help="Factor for the update.")

    return parser.parse_args()

def generate_db_names(directory: os.PathLike, num_dbs: int) -> list[str]:
    if not directory.endswith('/'):
        directory += '/'
    random_names = []
    for _ in range(num_dbs):
        random_names.append(''.join(random.choices(string.ascii_lowercase, k=5)))
    random_names_paths = [directory + random_name for random_name in random_names]
    return random_names_paths

def generate_files(names: list[str]) -> list[str]:
    file_names = [name + ".db" for name in names]
    for file_name in file_names:
        file = open(file_name, 'w')
        file.close()
    return file_names

def generate_sample_data() -> tuple:
    company_name = (''.join(random.choices(string.ascii_lowercase, k=6)))
    company_ticker = (''.join(random.choices(string.ascii_uppercase, k=4)))
    quantity = random.randint(1, 500)
    purchase_price = random.uniform(0.5, 1000.0)
    current_price = purchase_price * random.uniform(0.0001, 450.0)
    return (company_name, company_ticker, quantity, purchase_price, current_price)

def generate_dbs(dbs: list[str]) -> None:
    columns = {"name": "TEXT NOT NULL", "ticker": "TEXT PRIMARY KEY",
               "quantity": "INTEGER NOT NULL",
               "purchase_price": "REAL NOT NULL",
               "current_price": "REAL NOT NULL"}
    table_name = "investments"
    for db in dbs:
        conn = sqlite3.connect(db)
        cursor = conn.cursor()
        table_schema = f"""
            CREATE TABLE IF NOT EXISTS {table_name} (
                name {columns['name']},
                ticker {columns['ticker']},
                quantity {columns['quantity']},
                purchase_price {columns['purchase_price']},
                current_price {columns['purchase_price']}
            );
        """
        cursor.execute(table_schema)
        sample_data = []
        num_samples = random.randint(1, 12)
        for i in range(num_samples):
            sample_data.append(generate_sample_data())
        cursor.executemany(f"INSERT INTO {table_name} VALUES (?, ?, ?, ?, ?)", sample_data)
        conn.commit()
        conn.close()

def main(args: argparse.Namespace) -> int:
    if not os.path.exists(args.directory):
        os.makedirs(args.directory)
    db_names = generate_db_names(args.directory, args.number_dbs)
    full_file_names = generate_files(db_names)
    generate_dbs(full_file_names)
    return 0

if __name__ == '__main__':
    sys.exit(main(parse_args()))