--1
data Entity = Point Double Double |
              Circle Double Double Int |
              Container' [Entity]

entita :: Entity
entita = Container' [
        Container' [
            Point 5 5,
            Point 0.5 2
        ],
        Circle 1 1 5,
        Container' [
            Point 0.5 1,
            Circle 0.5 1 2,
            Circle 2 2 1
        ]
    ]

--Struktura
data Component = TextBox {name :: String, text :: String} |
                 Button {name :: String, value :: String} |
                 Container {name :: String, children :: [Component]}
                --deriving Show

gui:: Component
gui = Container "My App" [
    Container "Menu" [
        Button "btn_new" "New",
        Button "btn_open" "Open",
        Button "btn_close" "Close"
    ],
    Container "Body" [TextBox "textbox_1" "Some text goes here"],
    Container "Footer" []
    ]

--2
countButtons :: Component -> Int
countButtons (Button _ _) = 1
countButtons (TextBox _ _) = 0
countButtons (Container _ list) = sum (map countButtons list)

--3
addElement :: Component -> Component -> String -> Component
addElement (TextBox a b) _ _ = TextBox a b
addElement (Button a b) _ _ = Button a b
addElement (Container name list) component matchName | name == matchName = Container name (map (\a -> addElement a component matchName) list ++ [component]) 
                                                     | otherwise = Container name (map (\a -> addElement a component matchName) list)
