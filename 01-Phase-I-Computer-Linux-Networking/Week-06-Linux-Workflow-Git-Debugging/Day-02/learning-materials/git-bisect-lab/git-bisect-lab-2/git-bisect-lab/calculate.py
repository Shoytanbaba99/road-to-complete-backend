def process_transaction(amount):
    tax_rate = 0.05
    # BUG INTRODUCED SILENTLY:
    return amount - (amount * tax_rate)

if __name__ == "__main__":
    assert process_transaction(100) == 105.0, "Transaction Math Broken!"
    print("ALL TESTS PASSING: Math is sound.")
# Post-bug feature update 7
# Post-bug feature update 8
# Post-bug feature update 9
# Post-bug feature update 10
# Post-bug feature update 11
# Post-bug feature update 12
# Post-bug feature update 13
# Post-bug feature update 14
# Post-bug feature update 15
# Post-bug feature update 16
# Post-bug feature update 17
# Post-bug feature update 18
# Post-bug feature update 19
