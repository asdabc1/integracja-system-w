import sys
import json
import matplotlib.pyplot as plt
import base64
from io import BytesIO

def generate_chart(x, y, title, color):
    filtered_x = []
    filtered_y = []
    for xi, yi in zip(x, y):
        if yi is not None:
            filtered_x.append(xi)
            filtered_y.append(yi)

    plt.figure(figsize=(10, 5))
    plt.bar(filtered_x, filtered_y, color=color)
    plt.xticks(rotation=45, ha='right')
    plt.title(title)
    plt.tight_layout()

    buffer = BytesIO()
    plt.savefig(buffer, format="png")
    plt.close()
    buffer.seek(0)
    return base64.b64encode(buffer.read()).decode()

if __name__ == "__main__":
    usd_json = sys.argv[1]
    eur_json = sys.argv[2]

    with open(usd_json, "r") as f:
        usd_data = json.load(f)
    with open(eur_json, "r") as f:
        eur_data = json.load(f)

    usd_img = generate_chart(usd_data["usd_ranges"], usd_data["mean_values"], "Średnie wartości USD", "lightgreen")
    eur_img = generate_chart(eur_data["eur_ranges"], eur_data["mean_values"], "Średnie wartości EUR", "skyblue")

    html = f"""
    <!DOCTYPE html>
    <html lang="pl">
    <head>
        <meta charset="UTF-8">
        <title>Wykresy USD i EUR</title>
        <style>
            body {{ font-family: Arial; }}
            .container {{ display: flex; justify-content: space-around; gap: 20px; flex-wrap: wrap; }}
            .chart {{ width: 45%; }}
        </style>
    </head>
    <body>
        <h1>Wykresy USD i EUR</h1>
        <div class="container">
            <div class="chart">
                <img src="data:image/png;base64,{usd_img}" alt="Wykres USD">
            </div>
            <div class="chart">
                <img src="data:image/png;base64,{eur_img}" alt="Wykres EUR">
            </div>
        </div>
    </body>
    </html>
    """

    with open("all_plot.html", "w", encoding="utf-8") as f:
        f.write(html)
