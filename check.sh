#!/bin/bash
set -eo pipefail  # Exit on Pipeline failure

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
TEST_DIR="build/bin"
TIMEOUT=30  # Maximum time in seconds for each test
VERBOSE=${VERBOSE:-0}  # Verbose output if set to 1

# Logging functions
log_error() { echo -e "${RED}[ERROR]${NC} $*" >&2; }
log_success() { echo -e "${GREEN}[SUCCESS]${NC} $*"; }
log_info() { echo -e "${YELLOW}[INFO]${NC} $*"; }

# Check if timeout command exists
if command -v timeout &>/dev/null; then
    TIMEOUT_CMD="timeout"
else
    log_info "timeout command not found, tests won't be time-limited"
    TIMEOUT_CMD=""
fi

# Test execution function
run_test() {
    local test_path="$1"
    local test_name=$(basename "$test_path")
    local start_time=$(date +%s)
    local result=0

    log_info "Running test: $test_name"

    if [[ $VERBOSE -eq 1 ]]; then
        if [[ -n $TIMEOUT_CMD ]]; then
            $TIMEOUT_CMD $TIMEOUT "$test_path" || result=$?
        else
            "$test_path" || result=$?
        fi
    else
        if [[ -n $TIMEOUT_CMD ]]; then
            $TIMEOUT_CMD $TIMEOUT "$test_path" > /tmp/"$test_name".log 2>&1 || result=$?
        else
            "$test_path" > /tmp/"$test_name".log 2>&1 || result=$?
        fi
    fi

    local end_time=$(date +%s)
    local duration=$((end_time - start_time))

    if [[ $result -eq 0 ]]; then
        log_success "$test_name completed successfully (${duration}s)"
        [[ $VERBOSE -eq 0 ]] && rm -f /tmp/"$test_name".log
        return 0
    elif [[ $result -eq 124 ]]; then
        log_error "$test_name timed out after ${TIMEOUT}s"
    else
        log_error "$test_name failed with exit code $result"
    fi

    if [[ $VERBOSE -eq 0 ]]; then
        log_error "Test output:"
        cat /tmp/"$test_name".log
        rm -f /tmp/"$test_name".log
    fi
    return 1
}

# Check build directory
if [[ ! -d "$TEST_DIR" || -z "$(ls -A "$TEST_DIR")" ]]; then
    log_error "$TEST_DIR directory does not exist or is empty"
    log_error "Please run the build script first"
    exit 1
fi

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0
start_time=$(date +%s)

# Run all tests
for test in "$TEST_DIR"/*; do
    if [[ -x "$test" ]]; then
        total_tests=$((total_tests + 1))
        if run_test "$test"; then
            passed_tests=$((passed_tests + 1))
        else
            failed_tests=$((failed_tests + 1))
            [[ "${FAIL_FAST:-0}" -eq 1 ]] && exit 1
        fi
    else
        log_error "$(basename "$test") is not executable"
    fi
done

# Print summary
end_time=$(date +%s)
duration=$((end_time - start_time))

echo
log_info "Test Summary:"
echo "=============================="
echo "Total tests: $total_tests"
echo -e "Passed: ${GREEN}$passed_tests${NC}"
[[ $failed_tests -gt 0 ]] && echo -e "Failed: ${RED}$failed_tests${NC}" || echo "Failed: 0"
echo "Total time: ${duration}s"
echo "=============================="

# Exit with failure if any tests failed
[[ $failed_tests -gt 0 ]] && exit 1 || exit 0
