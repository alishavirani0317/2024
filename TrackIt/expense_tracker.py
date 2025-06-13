
from expense import Expense


def main():
    print(f"Running Expense Tracker") 

    # 1) get user to input their expense
    expense = get_user_expense()
    print(expense)

    # 2) write it to file 
    save_expense_to_file(expense)

    # 3) read file & summarize expenses
    summarize_expenses()

    pass

def get_user_expense():
    print("Getting User Expense")

    expense_name = input("Enter expense name: ") 
    expense_amount = float(input("Enter expense amount: "))

    #print(f"You've entered {expense_name}, {expense_amount}")
    
    # define list of categories from users to choose from
    expense_categories = [
        "🍔 Food", "🏠 Home", "💼 Work", "🎉 Fun", "✨ Misc"
    ]

    # present user with list and have each item have number on it
    while True:
        print("Select a category: ")
        for i, category_name in enumerate(expense_categories):
            print(f"    {i + 1}. {category_name}")

        value_range = f"[1 - {len(expense_categories)}]"
            
        selected_idx = int(input(f"Enter a category number {value_range}: ")) - 1

            
        if selected_idx in range(len(expense_categories)):
            selected_category = expense_categories[selected_idx]
            new_expense = Expense(name=expense_name, category=selected_category, amount = expense_amount)
            return new_expense
        else:
            print("Invalid Category. Please try again!")


def save_expense_to_file(expense):
    print("Saving User Expense: {expense}")

    # 1) open file (need to know file name )
    

def summarize_expenses():
    print("Summarizing User Expense")
    


# to make sure main() runs only when we run this file
# and not when we run it as part of another file 
if __name__ == "__main__":
    #only true when ran directly (not imported)
    main()
