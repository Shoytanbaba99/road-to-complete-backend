
if [ "$#" -ne 1 ]; then
    echo "Error: Missing command argument." >&2
    echo "Usage: $0 <command>" >&2
    exit 1
fi
unset AWS_SECRET_KEY

export PATH="/usr/bin:/bin"

export WRAPPER_SECURED="TRUE"
exec "$1"
