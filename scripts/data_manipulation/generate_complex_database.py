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
    parser.add_argument("--clear", dest='clear_existing', action='store_true', help="Delete existing DBs.")
    return parser.parse_args()

def generate_db_names(directory: os.PathLike, num_dbs: int) -> list[str]:
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

def generate_datetimestamp() -> str:

    def stringify(value: int) -> str:
        if value < 10:
            return "0" + str(value)
        return str(value)

    def get_random_day() -> str:
        months_with_extra = [1, 3, 5, 7, 8, 10, 12]
        day_upper = 30
        if month == 2:
            day_upper = 28
            if year % 4 == 0:
                day_upper = 29
        elif month in months_with_extra:
            day_upper = 31
        return stringify(random.randint(1, day_upper))

    sec_str = stringify(random.randint(0, 59))
    min_str = stringify(random.randint(0, 59))
    hour_str = stringify(random.randint(0, 23))
    month = random.randint(1, 12)
    month_str = stringify(month)
    year = random.randint(2020, 2024)
    year_str = str(year)
    day_str = get_random_day()
    return sec_str + min_str + hour_str + day_str + month_str + year_str

def generate_complex_inv_id() -> str:
    complex_inv_id = ""
    for _ in range(19):
        complex_inv_id += str(random.randint(0, 9))
    return complex_inv_id

def generate_sample_data_transactions_table() -> tuple:
    date = generate_datetimestamp()
    fees = random.uniform(0.001, 9.99)
    ccf = random.uniform(0.001, 2.99)
    ccr = random.uniform(0.0, 1.99)
    curr_list = ["EUR", "USD"]
    curr_idx = random.randint(0, 1)
    currency = curr_list[curr_idx]
    return (date, fees, currency, ccf, ccr)

def generate_sample_data_investments_table() -> tuple:
    company_name = (''.join(random.choices(string.ascii_lowercase, k=10)))
    company_ticker = (''.join(random.choices(string.ascii_uppercase, k=4)))
    quantity = random.uniform(0.1, 100.0)
    purchase_price = random.uniform(0.5, 1000.0)
    return (company_name, company_ticker, quantity, purchase_price)

def generate_dbs(dbs: list[str]) -> None:
    columns_investments = {"id": "TEXT PRIMARY KEY",
                           "name": "TEXT NOT NULL",
                           "ticker": "TEXT NOT NULL",
                           "quantity": "REAL NOT NULL",
                           "purchase_price": "REAL NOT NULL"}
    columns_transactions = {"id": "TEXT PRIMARY KEY",
                            "date": "TEXT NOT NULL",
                            "fees": "REAL NOT NULL",
                            "currency": "TEXT NOT NULL",
                            "currency_conversion_fees": "REAL NOT NULL",
                            "currency_conversion_rate": "REAL NOT NULL"}
    investments_table_name = "investments"
    transactions_table_name = "transactions"
    for db in dbs:
        conn = sqlite3.connect(db)
        cursor = conn.cursor()
        investments_table_schema = f"""
            CREATE TABLE IF NOT EXISTS {investments_table_name} (
                id {columns_transactions['id']},
                name {columns_investments['name']},
                ticker {columns_investments['ticker']},
                quantity {columns_investments['quantity']},
                purchase_price {columns_investments['purchase_price']}
            );
        """
        transactions_table_schema = f"""
            CREATE TABLE IF NOT EXISTS {transactions_table_name} (
                id {columns_transactions['id']},
                date {columns_transactions['date']},
                fees {columns_transactions['fees']},
                currency {columns_transactions['currency']},
                currency_conversion_fees {columns_transactions['currency_conversion_fees']},
                currency_conversion_rate {columns_transactions['currency_conversion_rate']}
            );
        """
        cursor.execute(investments_table_schema)
        cursor.execute(transactions_table_schema)
        sample_data_investments = []
        sample_data_transactions = []
        num_samples = random.randint(1, 12)
        for _ in range(num_samples):
            gen_id = generate_complex_inv_id()
            sdit = list(generate_sample_data_investments_table())
            sdtt = list(generate_sample_data_transactions_table())
            sdit.insert(0, gen_id)
            sdtt.insert(0, gen_id)
            sample_data_investments.append(tuple(sdit))
            sample_data_transactions.append(tuple(sdtt))
        cursor.executemany(f"INSERT INTO {investments_table_name} VALUES (?, ?, ?, ?, ?)",
                           sample_data_investments)
        cursor.executemany(f"INSERT INTO {transactions_table_name} VALUES (?, ?, ?, ?, ?, ?)",
                           sample_data_transactions)
        conn.commit()
        conn.close()

def main(args: argparse.Namespace) -> int:
    directory = args.directory
    if not os.path.exists(directory):
        os.makedirs(directory)
    if not directory.endswith('/'):
        directory += '/'
    if args.clear_existing:
        files_in_dir = os.listdir(directory)
        for f in files_in_dir:
            os.remove(directory + f)
    db_names = generate_db_names(directory, args.number_dbs)
    full_file_names = generate_files(db_names)
    generate_dbs(full_file_names)
    return 0

if __name__ == '__main__':
    sys.exit(main(parse_args()))