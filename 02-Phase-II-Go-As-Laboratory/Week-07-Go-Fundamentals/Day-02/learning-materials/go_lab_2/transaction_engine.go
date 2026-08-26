package main

import (
	"errors"
	"fmt"
)

const MaxLimit = 1000
const DefaultFee = 15

type Transaction struct {
	ID        int
	Amount    int
	Completed bool
}

func ExecuteTransaction(id int, balance int, amount int) (newBalance int, tx *Transaction, err error) {
	tx = &Transaction{ID: id, Amount: amount, Completed: false}
	defer func() {
		fmt.Printf("[AUDIT] Transaction ID: %d finalized with completed status: %b \n", id, tx.Completed)
	}()
	if amount <= 0 {
		return balance, tx, errors.New("invalid amount: must be greater than 0")
	}

	if amount+DefaultFee > balance {
		return balance, tx, errors.New("insufficient balance: amount + fee exceeds available balance")
	}

	newBalance = balance - amount - DefaultFee
	tx.Completed = true

	return newBalance, tx, nil
}

func Transaction_Engine() {
	balance := 1000
	fmt.Printf("Starting balance: %d\n", balance)
	fmt.Printf("Default fee: %d\n", DefaultFee)
	for i := 0; i < 5; i++ {
		amount := 100 + i*50
		newBalance, tx, err := ExecuteTransaction(i, balance, amount)

		switch {
		case err != nil:
			fmt.Printf(" Transaction %d FAILED: %v\n", i, err)
		case tx.Completed:
			fmt.Printf(" Transaction %d SUCCESS: Amount=%d, New Balance=%d\n", i, tx.Amount, newBalance)
			balance = newBalance
		default:
			fmt.Printf(" Transaction %d UNKNOWN STATE\n", i)
		}
	}
	fmt.Printf("Final balance: %d\n", balance)

}
