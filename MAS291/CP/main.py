import pandas as pd
import matplotlib.pyplot as plt
from statsmodels.tsa.arima.model import ARIMA
import warnings
warnings.filterwarnings("ignore")

# Sample data
data = {
    'Quarter': ['2024Q1', '2023Q4', '2023Q3', '2023Q2', '2023Q1', '2022Q4', '2022Q3', '2022Q2', '2022Q1',
                '2021Q4', '2021Q3', '2021Q2', '2021Q1', '2020Q4', '2020Q3', '2020Q2', '2020Q1'],
    'Sales': [14112, 15619, 15637, 15195, 14000, 15069, 16094, 14960, 13940, 15819, 16208, 15729, 13190,
              14425, 15563, 15495, 14150]
}

# Load data into DataFrame
df = pd.DataFrame(data)

# Convert Quarter to datetime


def convert_quarter_to_date(quarter_str):
    year, quarter = quarter_str[:4], quarter_str[4:]
    month = {'Q1': 1, 'Q2': 4, 'Q3': 7, 'Q4': 10}[quarter]
    return pd.Timestamp(f'{year}-{month:02d}-01')


df['Quarter'] = df['Quarter'].apply(convert_quarter_to_date)
df.set_index('Quarter', inplace=True)

# Fit ARIMA model
model = ARIMA(df['Sales'], order=(7, 1, 2))  # seasonal_order
fit = model.fit()

print(fit.summary())

# Forecast
forecast_steps = 4  # Predict the next 4 quarters
forecast = fit.forecast(steps=forecast_steps)
forecast_dates = pd.date_range(
    start=df.index[0], periods=forecast_steps + 1, freq='Q')[1:]

# Plot results
plt.figure(figsize=(10, 6))
plt.plot(df.index, df['Sales'], label='Actual Sales')
plt.plot(forecast_dates, forecast, label='Forecast', linestyle='-')
plt.xlabel('Quarter')
plt.ylabel('Sales')
plt.title('Vinamilk Quarterly Sales Forecast')
plt.legend()
plt.grid(True)
plt.savefig('forecast.png')
plt.show()
