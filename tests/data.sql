INSERT INTO 
	curr_exchange.currencies(code, full_name, sign) 
VALUES
	('USD', 'US Dollar', '$'),
	('EUR', 'Euro', '€'),	
	('RUB', 'Russian Ruble', '₽'),	
	('CUP', 'Cuban peso', '₱'),
	('CNY', 'Renminbi', '¥'),
	('RON', 'Romanian leu', 'L'),
	('BRL', 'Brazilian real', 'R$'),
	('IRR', 'Iranian rial', 'IR'),
	('TRY', 'Turkish lira', '₺'),
	('INR', 'Indian Rupee', '₹');
	
INSERT INTO 
	curr_exchange.exchange_rates(base_currency_id, target_currency_id, rate) 
VALUES
	(4, 3, 1.6),
	(6, 8, 1.3),
	(1, 7, 1.4),
	(1, 4, 1.8);