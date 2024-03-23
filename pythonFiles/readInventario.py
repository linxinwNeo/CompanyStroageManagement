import openpyxl
from model import Model, remove_lineSwitcher

def read_inventory_file(file_path, models):
    # Load the workbook
    workbook = openpyxl.load_workbook(file_path)

    # Get the first sheet
    sheet = workbook.active

    # Iterate through all rows in the sheet
    for row in sheet.iter_rows(values_only=True, min_row = 2):
        # Print each cell value in the row
        model = Model()
        model.MODEL_CODE = row[0]
        model.DESCRIPTION_SPAN = remove_lineSwitcher(row[1])
        model.NUM_INITIAL_BOXES = float(row[4])
        model.NUM_SOLD_BOXES = float(row[5])
        model.NUM_LEFT_ITEMS = float(row[6])
        model.NUM_LEFT_BOXES = float(row[7])
        model.NUM_ITEMS_PER_BOX = int(row[8])
        models[model.MODEL_CODE] = model 