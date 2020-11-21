from random import randrange, randint

numbers = ["+375291365418",
    "+375447290007",
    "+375297333998",
    "+375297263703",
    "+375291488999",
    "+375291822184",
    "+375291365387",
    "+375291222002",
    "+375448219245",
    "+375250124495",
    "8017449528692",
    "8017943750023"
]

first_names = [
    "Anton",
    "Nastya",
    "Nikita",
    "Andruliks",
    "Alex",
    "Vovchik",
    "Alena",
    "Sergey",
    "Pasha",
    "Sasha",
    "Misha",
]

last_names = [
    "Pashkevich",
    "Krasnov",
    "Dashkevich",
    "Martinovich",
    "Nikitko",
    "Kucherenko",
    "Rudoi",
    "Semin",
    "Elcin",
    "Ribbentrop",
    "Molotov",
]

middle_names = [
    "Victorovich",
    "Mikhailovich",
    "Glekovich",
    "Antonovich",
    "Alexandrovich",
    "Vitalievich",
    "Sergeevich",
    "Liavontsievich",
    "Roflovich",
]

streets = [
    "Lobanka",
    "Jopnaya",
    "Prityskogo",
    "Pushkina",
    "Molodezhnaya",
    "Sportivnaya",
    "Lesnaya",
    "Kalvariyskaya",
    "Yakuba Kolasa",
    "Surganova",
    "Gikalo",
]

with open("phonebook.db", "w") as f:
    for i in range(0, 2000):
        result = ""
        result += str(numbers[randrange(0, len(numbers))]) + ";"
        result += str(last_names[randrange(0, len(last_names))]) + ";"
        result += str(first_names[randrange(0, len(first_names))]) + ";"
        result += str(middle_names[randrange(0, len(middle_names))]) + ";"
        result += str(streets[randrange(0, len(streets))]) + ";"
        result += str(randint(0, 100)) + ";"
        result += str(randint(0, 100)) + ";\n"

        f.write(result)
