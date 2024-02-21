import os
import sys
import sqlite3
import argparse

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Update files in a directory.")
    parser.add_argument("database_type", choices=["db", "text"], help="Type of database to be updated.")
    parser.add_argument("directory", help="Path to the directory containing files to be updated.")
    parser.add_argument("update_type", choices=["add_column"], help="Type of update to be performed.")
    parser.add_argument("--factor", type=float, default=1.0, help="Factor for the update.")

    return parser.parse_args()

def update_database_files(directory_path: str, update_type: str, update_factor: float) -> None:
    database_files = [file for file in os.listdir(directory_path) if file.endswith(".db")]
    for file in database_files:
        database_path = os.path.join(directory_path, file)
        try:
            connection = sqlite3.connect(database_path)
            cursor = connection.cursor()
            if update_type == "add_column":
                cursor.execute("ALTER TABLE investments ADD COLUMN current_price REAL NOT NULL DEFAULT 0.0")
                cursor.execute(f"UPDATE investments SET current_price = {update_factor} * purchase_price")
            connection.commit()
        except sqlite3.Error as ex:
            print(f"Error updating {file}: {ex}")
        finally:
            if connection:
                connection.close()

def main(args: argparse.Namespace) -> int:
    if args.database_type == "db":
        update_database_files(args.directory, args.update_type, args.factor)
    else:
        print(f"No proper handling for database type: {args.database_type}")
        return 1
    return 0

if __name__ == '__main__':
    sys.exit(main(parse_args()))
