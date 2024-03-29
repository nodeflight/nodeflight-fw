#!/bin/bash

BASEDIR=$(git rev-parse --show-toplevel)
cd $BASEDIR

for F in $(find "src" -name '*.[ch]' -not -path 'src/vendor/*'); do
	if grep -q 'You should have received a copy of the GNU General Public License' ${F}; then
		printf "    %-45s %s\n" "${F}" "$(grep 'Copyright (C)' "${F}" | sed 's/^.*(C)//')"
	else
		mv "${F}" "${F}_orig"
		cat > "${F}" <<EOF
/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

EOF
		cat "${F}_orig" >> "${F}"
		rm "${F}_orig"
		printf " >> %-45s %s\n" "${F}" "$(grep 'Copyright (C)' "${F}" | sed 's/^.*(C)//')"
	fi
done

echo "Verify git diff before commit!"
