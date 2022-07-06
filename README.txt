=========Background============
Best Quality Furniture

-Wholesale retail company that offers extensive range of beautiful and magificent furnishings for your household or office

(https://bestqualityfurn.com/)
===============================

=========Purpose===============
> Update the existing product's price and inStock Status on Best Quality Furniture's website
- Match Product Names in Exported Accuterm's CSV file to the Website's Exported Inventory CSV file and transfer its Price,
inStock Status (Based on how much is in stock and its status(discontinuing vs continuing)

> Generate List of Products to Delete off Website
- Find products that are discontinuing and no longer in stock. Match these Product Names in Exported Accuterm's CSV file to 
Website's Exported Inventory CSV file and generate CSV file "Items To Delete off Website.csv".

> Generate List of Products to Add onto Website
- Find products that are on Accuterm and either: Continuing or Discontinuing(Only if in Stock). Match these Product Names in 
Exported Accuterm's CSV file to Website's Exported Inventory CSV file and generate CSV file "Items To Add onto
Website.csv", if match isn't found.
- (NOTE: Some items may be added here because of incorrect names on the website's end)

> Generate List of Products in Accuterm that encountered runtime error
- If a program throws an invalid_argument error, it will generate a CSV file "Accuterm Items with Error Naming Conventions.csv"
(Usually due to commas in discriptions, which causes the code to read into incorrect columns)

> Inputs: Accuterm's Exported Data as CSV files(Running.csv + Slow Seller.csv + Discontinued.csv + Close Out.csv)
