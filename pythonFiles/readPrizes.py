import openpyxl
from model import Model, remove_non_alnum_hyphen, remove_lineSwitcher

def read_Prizes_file(file_path, models):
    # Load the workbook
    workbook = openpyxl.load_workbook(file_path)

    # Get the first sheet
    firstsheet = third_worksheets = workbook.worksheets[0]

    # Iterate through all rows in the sheet
    for row in firstsheet.iter_rows(values_only=True, min_row = 2):
        model_code = remove_non_alnum_hyphen(str(row[0])) # remove chinese in code
        if model_code in models:
            model = models[model_code]
        else: continue # if not found this model, ignore
        model.DESCRIPTION_CN = remove_lineSwitcher(row[1]) # description_Chinese
        model.PRIZE = row[10] # prize

    # we only read the first sheet
    # third_worksheets = workbook.worksheets[2]
    # # Iterate through all rows in the sheet
    # for row in third_worksheets.iter_rows(values_only=True, min_row = 2):
    #     model_code = remove_non_alnum_hyphen(str(row[0])) # remove chinese in code
    #     if model_code in models:
    #         model = models[model_code]
    #     else: continue # if not found this model, ignore

    #     model.PRIZE = row[10] # prize