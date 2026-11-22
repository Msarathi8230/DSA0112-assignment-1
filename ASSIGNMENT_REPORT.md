# Assignment 1 Report
## Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System

**Student:** Enter full name  
**Register number:** Enter register number  
**Department:** Enter department  
**Course outcome:** CO1  
**Total:** 50 marks

> Complete the student fields and personally rerun/verify all programs before submission. The execution logs in each task folder are captured from the preparation environment; they are not substitutes for your own required output screenshots.

## Assumptions and common rules
- Money is stored as integer paise (100 paise = Rs 1).
- Base fares: Inner Rs 10, Middle Rs 20, Outer Rs 30.
- Peak windows are half-open intervals: 08:00–09:59 and 17:00–19:59. Thus exactly 10:00 and 20:00 are off-peak.
- Peak surcharge is 20%; at least Rs 50 must remain after deduction.
- Task 12 weekend discount is 10% and replaces (does not combine with) the peak surcharge.
- The monthly pass covers two selected zones and makes covered journeys free.

## Task 1 – Object-oriented problem analysis
**Objects and responsibilities**
- `SmartCard`: owns private balance; exposes `canTravel`, `deduct`, and balance query. It enforces the Rs 50 minimum invariant.
- `Passenger`: stores passenger category/identity; student or senior concession policy can be added as a separate policy object. The assignment does not specify concession percentages, so no percentage is invented in the implementation.
- `Journey`: records origin/destination or farthest zone, travel time, and date; asks the fare policy for the fare.
- `Zone`: represents Inner, Middle and Outer as a finite set.
- `FareTable`: centralizes base fares and surcharge parameters.

Procedural code stores values separately and passes them to free functions. OOP groups state and behaviour, restricts direct balance changes and isolates fare rules. Encapsulation protects balance; abstraction exposes fare/deduction operations; inheritance could model StudentPassenger and SeniorPassenger from Passenger; polymorphism could provide different concession policy implementations.

**Journey invariants:** zone must be valid; hour must be 0–23; fare must be non-negative; deduction occurs only if resulting balance is at least 5000 paise; failed journeys do not change balance.

**Alternative/complexity:** procedural and OOP fare lookup/deduction are O(1) time and O(1) balance space per card. A rule change in procedural code may require editing multiple functions/call sites; centralized policy/class design localizes the change.

## Task 2 – Tokens, identifiers and constants
Examples: keywords (`class`, `const`, `return`), identifiers (`SmartCard`, `balance`), constants (`5000`, `PEAK_PERCENT`), operators (`>=`, `-`, `&&`), punctuators (`{}`, `;`, `()`).
`#define` is textual substitution without type or scope; `const` creates read-only typed objects; `constexpr` enables compile-time evaluation and type checking. `enum class Zone` is scoped and does not implicitly convert to int. Prefer `constexpr` for compile-time fare constants. Identifier convention: descriptive `PascalCase` for types and `camelCase` for functions/variables; uppercase names for constants.

**Alternative:** macros normally occupy no storage and constexpr constants often also need no runtime storage when optimized/constant-folded. Execution time is effectively unchanged; constexpr provides stronger type checking and scope.

## Task 3 – Data types and compatibility
| Value | Recommended type | Reason |
|---|---|---|
| Fare/balance in paise | `long long` | Exact integer arithmetic and large totals |
| Peak flag | `bool` | Two-state value |
| Zone | `enum class Zone` | Valid named alternatives |
| Hour | `int` | Small signed range |
| Daily journeys | `long long` | Network-scale count |
| Display rupees | `double` only for formatting if needed | Never use as authoritative money |

Use `sizeof` output in Task03 for the actual platform. `int` is commonly 32-bit; `long long` is at least 64-bit. `<climits>` reports `INT_MIN`, `INT_MAX`, `LLONG_MIN`, `LLONG_MAX`. At three million journeys of Rs 30 each, total is Rs 90,000,000 (9 crore) = 9,000,000,000 paise, beyond a signed 32-bit int. Integer paise avoids binary floating representation such as `0.1 + 0.2` not being exactly 0.3.

**Alternative:** double rupees and long long paise are commonly both 8 bytes and additions are O(1), but floating point may accumulate rounding error. Integer paise is exact for whole-paise fares.

## Task 4 – Implicit conversions
**Prediction before execution:**
```text
3 3.5
C 68
1 true
s >= u
```
`a/b` is integer division (3) before conversion to double. Explicit cast makes division floating-point (3.5). `ch` is printed as a character (`C`); `ch+1` promotes char to int (68). Integer 5 converts to bool true; bool prints as 1 then `true` after `boolalpha`. In `s<u`, signed/unsigned usual arithmetic conversions convert `s=-1` to a large unsigned value, so the comparison is false. Explicit casts and brace initialization expose potentially unsafe conversions; they do not materially change runtime time or storage.

## Task 5 – Operators and precedence
**Prediction before execution:**
```text
11 16
10 6 4
6 4
3 1 -3 -1
18
```
Multiplication binds more tightly than addition. Parentheses force the alternate grouping. Post-increment yields old x (5), pre-increment changes y to 4 and yields 4; sum is 9, then x=6 and y=4. Conditional operator selects x=6. `%` and `*` have equal multiplicative precedence and associate left-to-right: `(6%4)*2=4`. Integer division truncates toward zero; remainder has dividend's sign. The loop visits 0,3,6,9, sum 18. Peak expression: `(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)`. A 24-element bool lookup table is also O(1) per check but uses 24 bytes and avoids repeated comparisons.

## Task 6 – Control structures and pseudocode
**Pseudocode**
```text
ALGORITHM ComputeFare(zone, hour)
  IF zone is invalid OR hour outside 0..23 THEN RETURN invalid
  SWITCH zone
    CASE Inner: base <- 1000
    CASE Middle: base <- 2000
    CASE Outer: base <- 3000
  END SWITCH
  peak <- (hour >= 8 AND hour < 10) OR (hour >= 17 AND hour < 20)
  IF peak THEN fare <- base + (base * 20 / 100)
  ELSE fare <- base
  RETURN fare
END
```
Use switch for mutually exclusive zones, if/else for validation and peak policy, and a range-based for loop when iterating a known collection. `break` prevents switch fall-through; `continue` can skip invalid input records. A switch with no matching case and no default executes no branch. The completed program's expected output is the three lines shown in Task06 execution log.

**Alternative:** constexpr array lookup is O(1), uses storage for three fare values, and adding a fourth zone requires another element plus enum/validation updates. A switch requires a new case.

## Task 7 – Procedural versus OOP
Task07 contains both approaches. Both use O(1) time and O(1) balance storage. Private balance plus `deduct()` prevents unrelated code from bypassing the minimum-balance rule. Public balance would use the same time and space but any caller could assign a negative or too-low balance. Centralizing the minimum in one constant makes revision safer.

## Task 8 – Safe input/output
Task08 checks stream extraction, uses `cin.clear()` and `cin.ignore()` after invalid input, validates the hour range, and uses `fixed`/`setprecision`. `setw` applies to the next formatted field and `setfill` controls padding. `getline` plus `stoi` also takes O(L) for a line of length L, needs a line buffer, and supports try/catch recovery; direct extraction is simpler for token-oriented numeric input.

## Task 9 – Implementation and test evidence
Task09 runs a table-driven suite for off-peak/peak and boundary hours. Expected cases include 08:00 peak, 10:00 off-peak, 17:00 peak and 20:00 off-peak. Invalid zone/hour returns -1. A balance of Rs 60 paying Rs 10 leaves exactly Rs 50 and is allowed; a fare leaving less than Rs 50 is denied. Extend the table with invalid recharge and non-numeric hour cases when testing the interactive gate. For t tests, table-driven execution is O(t) time and O(t) table space; adding a case is a new row.

## Task 10 – Expected versus execution
For a fixed Rs 30 fare, totals are:
| Journeys | Total rupees | Total paise |
|---:|---:|---:|
| 20,000 | 600,000 | 60,000,000 |
| 500,000 | 15,000,000 | 1,500,000,000 |
| 3,000,000 | 90,000,000 | 9,000,000,000 |

The final paise total exceeds 32-bit signed int. `long long` safely holds it. `double` can represent these integer rupee totals exactly at this magnitude, but is not a safe general money representation for fractional binary values. Addition is O(1) for all types; storage is typically 4 bytes for int and 8 for long long/double. Record exact compiler warnings from each execution log; no warning is not proof of correctness.

## Task 11 – Final recommendation
Retain the OOP boundary and integer paise, but centralize zone fares and time/discount policies. Use `long long`, `enum class`, `constexpr` constants, private SmartCard balance and validation. A fare revision changes one table/policy definition and tests. Limitations: hard-coded configuration requires rebuild; concession rates and passenger eligibility need explicit business rules; persistence, concurrency, refunds and audit logs are outside this small assignment. O(1) work per journey scales linearly with journey count, so processing n journeys is O(n).

## Task 12 – Design challenge: modify the design
**Impact analysis:** add `Airport` to Zone and Rs 60 (6000 paise) to fare data; update any fixed-size arrays, validation, switch statements and tests. Add weekend flag/policy and a Pass class storing two covered zones. SmartCard remains responsible for stored-value balance, while pass eligibility is checked before charging.

**Worked examples:** Airport weekday peak = Rs 60 × 1.20 = Rs 72. Airport weekend = Rs 60 × 0.90 = Rs 54, not Rs 64.80; weekend discount replaces peak surcharge. A covered zone under an active monthly pass costs Rs 0.

**Adaptation:** Task12 uses a four-entry constexpr array, explicit weekend/peak precedence, and a Pass class. A lookup table is easier to extend than a switch as zones grow. The revised fare lookup is O(1); pass membership is O(1) for two stored zones. The pass uses O(1) space. Retain SmartCard for stored-value journeys and add a separate pass policy rather than overloading balance logic.

## Complexity summary
| Operation | Time | Extra space |
|---|---:|---:|
| Fare lookup | O(1) | O(1) |
| Peak-hour test | O(1) | O(1), or 24 bytes for lookup table |
| Card balance check/deduction | O(1) | O(1) per card |
| Process n journeys | O(n) | O(1) streaming, excluding stored records |
| Run t test cases | O(t) | O(t) for test table |

## Final submission checklist
- Fill student details and GitHub URL.
- Recompile and run every source file on your own computer.
- Save genuine terminal screenshots in each task folder if required.
- Verify predicted output sections were written before running Tasks 4 and 5.
- Commit each task separately; include the actual GitHub commit history.
- Submit repository link and this report through Google Classroom.
