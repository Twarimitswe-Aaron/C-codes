import re
import sqlite3
from typing import List, Dict, Optional

class SchemaGenerator:
    def __init__(self, db_path: str = "database.db"):
        """Initialize the schema generator with a database path."""
        self.db_path = db_path
        self.conn = None
        self.cursor = None

    def connect(self):
        """Connect to the SQLite database."""
        self.conn = sqlite3.connect(self.db_path)
        self.cursor = self.conn.cursor()

    def close(self):
        """Close the database connection."""
        if self.conn:
            self.conn.close()

    def parse_schema_file(self, file_path: str) -> List[Dict]:
        """Parse the schema file and return a list of table definitions."""
        tables = []
        current_table = None

        with open(file_path, 'r') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue

                # Check for table definition
                if line.startswith('TABLE'):
                    if current_table:
                        tables.append(current_table)
                    current_table = {
                        'name': line.split()[1],
                        'columns': []
                    }
                # Parse column definition
                elif current_table and line:
                    column_def = self._parse_column(line)
                    if column_def:
                        current_table['columns'].append(column_def)

        if current_table:
            tables.append(current_table)

        return tables

    def _parse_column(self, line: str) -> Optional[Dict]:
        """Parse a single column definition line."""
        # Expected format: column_name type [constraints]
        parts = line.split()
        if len(parts) < 2:
            return None

        column = {
            'name': parts[0],
            'type': parts[1].upper(),
            'constraints': []
        }

        # Parse constraints
        for part in parts[2:]:
            if part.upper() in ['PRIMARY', 'KEY', 'NOT', 'NULL', 'UNIQUE']:
                column['constraints'].append(part.upper())

        return column

    def generate_create_statements(self, tables: List[Dict]) -> List[str]:
        """Generate CREATE TABLE statements from parsed table definitions."""
        create_statements = []
        
        for table in tables:
            columns = []
            for col in table['columns']:
                column_def = f"{col['name']} {col['type']}"
                if col['constraints']:
                    column_def += " " + " ".join(col['constraints'])
                columns.append(column_def)
            
            create_stmt = f"CREATE TABLE {table['name']} (\n    " + ",\n    ".join(columns) + "\n);"
            create_statements.append(create_stmt)
        
        return create_statements

    def execute_schema(self, create_statements: List[str]):
        """Execute the CREATE TABLE statements to create the database schema."""
        if not self.conn:
            self.connect()

        for statement in create_statements:
            try:
                self.cursor.execute(statement)
            except sqlite3.Error as e:
                print(f"Error executing statement: {e}")
                print(f"Statement: {statement}")
                raise

        self.conn.commit()

def main():
    # Example usage
    generator = SchemaGenerator()
    
    try:
        # Parse the schema file
        tables = generator.parse_schema_file('schema.txt')
        
        # Generate CREATE statements
        create_statements = generator.generate_create_statements(tables)
        
        # Print the generated statements
        print("Generated CREATE TABLE statements:")
        for stmt in create_statements:
            print("\n" + stmt)
        
        # Ask user if they want to execute the statements
        response = input("\nDo you want to execute these statements? (y/n): ")
        if response.lower() == 'y':
            generator.execute_schema(create_statements)
            print("Schema created successfully!")
    
    except Exception as e:
        print(f"Error: {e}")
    finally:
        generator.close()

if __name__ == "__main__":
    main() 