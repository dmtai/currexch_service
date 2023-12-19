DROP SCHEMA IF EXISTS curr_exchange CASCADE;
CREATE SCHEMA IF NOT EXISTS curr_exchange;

CREATE TABLE IF NOT EXISTS curr_exchange.currencies (
	currency_id SERIAL,
	code VARCHAR(3) NOT NULL,
	full_name VARCHAR(255) NOT NULL,
	sign VARCHAR(255) NOT NULL,
	updated_at timestamp with time zone NOT NULL DEFAULT NOW(),
	CONSTRAINT pk_currencies PRIMARY KEY(currency_id),
	CONSTRAINT uniq_code UNIQUE(code)
);

CREATE TABLE IF NOT EXISTS curr_exchange.exchange_rates (
	exchange_rate_id SERIAL,
	base_currency_id INT NOT NULL,
	target_currency_id INT NOT NULL,
	rate NUMERIC(8, 6) NOT NULL,
	updated_at timestamp with time zone NOT NULL DEFAULT NOW(),
	CONSTRAINT pk_exchange_rate_id PRIMARY KEY(exchange_rate_id),
	CONSTRAINT fk_base_currency_id FOREIGN KEY(base_currency_id) REFERENCES curr_exchange.currencies(currency_id),
	CONSTRAINT fk_target_currency_id FOREIGN KEY(target_currency_id) REFERENCES curr_exchange.currencies(currency_id),
	CONSTRAINT uniq_curr_pair UNIQUE(base_currency_id, target_currency_id)
);

CREATE OR REPLACE FUNCTION curr_exchange.update_updated_at()
    RETURNS trigger
AS $$
	BEGIN NEW.updated_at = NOW(); 
	RETURN NEW; 
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER tr_update_currencies_updated_at
  BEFORE UPDATE 
  ON curr_exchange.currencies
  FOR EACH ROW
  EXECUTE FUNCTION curr_exchange.update_updated_at();

CREATE TRIGGER tr_update_exchange_rates_updated_at
  BEFORE UPDATE 
  ON curr_exchange.exchange_rates
  FOR EACH ROW
  EXECUTE FUNCTION curr_exchange.update_updated_at();