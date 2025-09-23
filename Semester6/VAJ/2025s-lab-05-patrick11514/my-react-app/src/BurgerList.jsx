/**
 * @param {{
 * name: string;
 * count: number;
 * }} props
 **/
export default function BurgerList(props) {
    const { name, count } = props;

    return (
        <ul>
            {Array.from({ length: count }).map((_, i) => (
                <li key={i}>{name} is the great burger!</li>
            ))}
        </ul>
    );
}
