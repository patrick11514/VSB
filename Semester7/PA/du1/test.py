def load_data():
    path = "input.txt"
    with open(path) as f:
        lines = f.readlines()
        number_of_items = int(lines[0])
        widths = lines[1]
        widths = [int(x) for x in widths.split()]
        tmp = lines[2:(2 + number_of_items)]
        data = []
        for line in tmp:
            row = []
            parts = line.split()
            for p in parts:
                row.append(float(p))
            data.append(row)

    return [widths, data]

print(load_data())


def load_data():
    path = "Y-10_t.txt"
    with open(path) as f:
        lines = f.readlines()
        number_of_items = int(lines[0])
        widths = lines[1]
        widths = [int(x) for x in widths.split()]
        tmp = lines[2:(2 + number_of_items)]
        data = []
        for line in tmp:
            row = []
            parts = line.split()
            for p in parts:
                row.append(float(p))
            data.append(row)

    n = len(data)
    for i in range(n):
        for j in range(i+1, n):
            data[j][i] = data[i][j]

    return [widths, data]