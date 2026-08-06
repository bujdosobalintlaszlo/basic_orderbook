#pragma once
enum class OrderType {GoodTillCancel,FillAndKill,FillOrKill,GoodForDay,Market,PostOnly,IceBerg};
/*
 * Notes for ordertypes:
 * ___NOT LISTED IN THE BOOK___
 * - Market: Executed immediately with the best possible offer from the book. Remaining amount is cancelled (or rejected if no liquidity). Won't get listed in the book.
 * - FillAndKill (Immediate-Or-Cancel -> IOC): Fills as much as it can at the given limit price or better. Remaining amount is cancelled immediately. Won't sit in the book.
 * - FillOrKill (FOK): If the order can be executed FULLY immediately at the given price or better, execute it. Otherwise cancel/reject the entire order. Won't sit in the book.
 * 
 * ___LISTED IN THE BOOK___
 * - GoodTillCancel (GTC): Stays in the book until fully filled or explicitly cancelled by the user.
 * - GoodForDay (GFD): Stays in the book until the end of the trading day (then auto-cancelled by the exchange).
 * 
 * - PostOnly: Must ONLY act as a Maker (sit in the book). If it would execute immediately against an existing order (acting as a Taker), it gets cancelled instead of placed.
 * - IceBerg: A large order broken into smaller visible segments (e.g., 10,000 total size, but only displays 100 at a time in the book). Once a visible slice is filled, the next slice is pushed to the book.
 */
